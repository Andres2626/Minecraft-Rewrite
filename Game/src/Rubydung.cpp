#include "Rubydung.h"

Rubydung::Rubydung()
	: m_InternalWindow(Application::GetInstance().GetWindow()),
	m_Props(m_InternalWindow.GetProps())
{
	m_Timer = new Timer();
	m_Last = { m_Props.x / 2, m_Props.y / 2 };
}

Rubydung::~Rubydung() 
{
	delete m_Player;
	delete m_Level;
	delete m_Selector;
	delete m_SShader;
	delete m_CShader;
	delete m_Timer;
}

void Rubydung::Init() 
{
	Default::Init();

	if (!m_Icon.LoadFromFile("assets/Internal/win_icon.png")) 
		mc_fatal("failed to open image file: \"%s\"\n", m_Icon.path);

	m_InternalWindow.SetIcon(m_Icon);
	m_Icon.Free();

	/* load shader programs */
	m_CShader = new Shader("assets/Shaders/chunk.shader");
	m_SShader = new Shader("assets/Shaders/selector.shader");

	/* initialize chunk renderer */
	Renderer::DepthFunc(DepthValue::LEQUAL);
	Renderer::Enable(GL_DEPTH_TEST);
	Renderer::ClearColor({ 0.5f, 0.8f, 1.0f });

	/* create player selector */
	m_Selector = new Selector();

	/* create level */
	m_Level = new Level({ 256, 256, 64 });

	/* setup steve */
	m_Player = new Player(m_Level);
	m_Player->Cam.aspect = (float)m_Props.x / (float)m_Props.y;

	/* load texture */
	if (!m_TerrainTexture.LoadFromFile("assets/terrain.png", GL_NEAREST))
		mc_fatal("failed to open texture file: \"%s\"\n", m_TerrainTexture.path);
}

void Rubydung::OnUpdate(Timestep& ts) 
{
	Default::OnUpdate(ts);

	m_Player->Update();

	if (Input::IsKeyPressed(MC_KEY_ESCAPE)) {
		m_Level->Save();
		Application::GetInstance().Stop();
	}
}

void Rubydung::OnKeyPressed(int key) 
{
	switch (key) {
	case MC_KEY_ESCAPE:
		m_Level->Save();
		Application::GetInstance().Stop();
		break;
	case MC_KEY_ENTER:
		m_Level->Save();
		break;
	default:
		break;
	}
}

void Rubydung::OnCursorMoved(int& x, int& y) 
{
	vec2 pos(x, y);
	vec2 offset(pos.x - m_Last.x, m_Last.y - pos.y);
	m_Last = pos;

	m_Player->MouseMove(offset);
}

void Rubydung::OnEvent(Event& ev) 
{
	Default::OnEvent(ev);

	while (m_InternalWindow.GetEvent(ev)) {
		Input::ProcessEvent(ev);

		switch (ev.type) {
		case GLEQ_KEY_PRESSED:
			this->OnKeyPressed(ev.keyboard.key);
			break;
		case GLEQ_WINDOW_RESIZED:
			Renderer::Viewport({ 0, 0 }, { ev.size.width, ev.size.height });
			m_Player->Cam.aspect = (float)ev.size.width / (float)ev.size.height;
			break;
		case GLEQ_CURSOR_MOVED:
			OnCursorMoved(ev.pos.x, ev.pos.y);
			break;
		}
		m_InternalWindow.FreeEvent(ev);
	}
}

void Rubydung::OnRender() 
{
	Default::OnRender();

	/* 
	 * GAME RENDER MAIN PROCESS:
	 * -- Enable chunk shader
	 * -- Render player (Update Camera vectors)
	 * -- Set VP matrix in chunk shader
	 * -- Update player
	 * -- Set fog uniforms in the chunk shader
	 * -- Render level
	 * -- Enable selector shader
	 * -- PlayerPick process
	 */
	m_CShader->Bind();
	m_Player->Render();

	mat4 VP = m_Player->Cam.GetProj() * m_Player->Cam.GetView();
	m_CShader->Set4x4("s_VP", VP);

	m_CShader->SetVec3("s_cpos", m_Player->Cam.pos);
	m_CShader->SetVec4("s_fcolor", { 14 / 255.0f, 11 / 255.0f, 10 / 255.0f, 1.0f });
	m_CShader->SetFloat("s_fstart", -10.0f);
	m_CShader->SetFloat("s_fend", 20.0f);

	m_Level->Render(m_CShader, m_Player);

	m_SShader->Bind();
	m_SShader->Set4x4("s_VP", m_Player->Cam.GetProj() * m_Player->Cam.GetView());

	PlayerPick();
}

void Rubydung::OnTick() 
{
	Default::OnTick();

#ifndef NDEBUG
	mc_info("fps: %i, ups: %i, cups: %i\n", Application::GetInstance().GetFPS(), Application::GetInstance().GetUPS(), m_Level->GetUpdates());
#else
	printf("%i fps, %i\n", Application::GetInstance().GetFPS(), lev->GetUpdates());
#endif
}

void Rubydung::OnSuspended() {
	Default::OnSuspended();
}

bool Rubydung::Raycast(const vec3& org, const vec3& dir, Hitresult& ret) 
{
	vec3 d = dir;
	vec3 lpoint = org;
	ivec3 lblock = floor(org);
	Ray camray;

	for (float t = 0.0f; t < 5.0f; t += 0.1f) {
		/* get point by parametric equation f(t) = dir * t + pos */
		vec3 equation = camray.GetRay(t, org, d);

		/* impact block */
		ivec3 blockpos = floor(equation);

		if (blockpos != lblock && m_Level->IsSolidTile(blockpos)) {
			ivec3 normal = lblock - blockpos;

			/* obtain block face normal */
			if (normal == ivec3(0)) {
				vec3 ad = abs(d);

				if (ad.x >= ad.y && ad.x >= ad.z)
					normal = ivec3((dir.x > 0) ? -1 : 1, 0, 0);
				else if (ad.y >= ad.x && ad.y >= ad.z)
					normal = ivec3(0, (dir.y > 0) ? -1 : 1, 0);
				else
					normal = ivec3(0, 0, (dir.z > 0) ? -1 : 1);
			}

			ret.block = blockpos;
			ret.dis = t;
			ret.face = normal;
			ret.point = equation;

			return true;
		}

		lpoint = equation;
		lblock = blockpos;
	}

	return false;

}

void Rubydung::PlayerPick() 
{
	/* Obtain the vectors of parametric equation f(t) = ray * d + pos */
	vec3 ray = m_Player->Cam.front;
	vec3 org = m_Player->Cam.pos;

	/* Result of the hit */
	Hitresult ret;

	/* The camera hit solid block? */
	bool hit = Raycast(org, ray, ret);

	if (hit) {
		/* Render player selector */
		m_Selector->SetHit(ret);
		m_Selector->Render(m_Player->Cam, m_SShader, m_Timer->ElapsedMillis());

		/* Get mouse button status */
		bool left = Input::IsMouseButtonPressed(MC_MOUSE_BUTTON_1);
		bool right = Input::IsMouseButtonPressed(MC_MOUSE_BUTTON_2);

		/* Avoid click spam */
		if (left && !m_MouseLeft)
			m_Level->SetTile(ret.block, 0); /* delete tile */

		if (right && !m_MouseRight)
			m_Level->SetTile(ret.block + ret.face, 1); /* set tile */

		/* Update mouse state */
		m_MouseLeft = left;
		m_MouseRight = right;
	}
}
	