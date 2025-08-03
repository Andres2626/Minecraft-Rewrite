#include "Rubydung.h"

Rubydung::Rubydung()
	: InternalWindow(Application::GetInstance().GetWindow()),
	props(InternalWindow.GetProps())
{
	this->tm = new Timer();
	this->last = { props.x / 2, props.y / 2 };
}

Rubydung::~Rubydung() 
{
	delete this->steve;
	delete this->lev;
	delete this->py_sel;
	delete this->sh_sel;
	delete this->sh_chunk;
	delete this->tm;
}

void Rubydung::Init() 
{
	DefaultLayer::Init();

	if (!icon.LoadFromFile("assets/Internal/win_icon.png")) {
		RD_FATAL << "Error loading window icon.";
	}
	this->InternalWindow.SetIcon(icon);
	icon.Free();

	/* load shader programs */
	sh_chunk = new Shader("assets/Shaders/chunk.shader");
	sh_sel = new Shader("assets/Shaders/selector.shader");

	/* initialize chunk renderer */
	Renderer::DepthFunc(DepthValue::LEQUAL);
	Renderer::Enable(GL_DEPTH_TEST);
	Renderer::ClearColor({ 0.5f, 0.8f, 1.0f });

	/* create player selector */
	py_sel = new Selector();

	/* create level */
	lev = new Level({ 256, 256, 64 });

	/* setup player */
	steve = new Player(lev);
	steve->cam.aspect = (float)props.x / (float)props.y;

	/* load texture */
	if (!tex.LoadFromFile("assets/terrain.png", GL_NEAREST)) 
		RD_FATAL << "Error loading " << tex.path;
}

void Rubydung::OnUpdate(Timestep& ts) 
{
	DefaultLayer::OnUpdate(ts);

	if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
		lev->Save();
		Application::GetInstance().Stop();
	}

	steve->tick();
}

void Rubydung::OnKeyPressed(int key) 
{
	switch (key) {
	case GLFW_KEY_ESCAPE:
		lev->Save();
		Application::GetInstance().Stop();
		break;
	case GLFW_KEY_ENTER:
		lev->Save();
		break;
	default:
		break;
	}
}

void Rubydung::OnCursorMoved(int& x, int& y) 
{
	vec2 pos(x, y);
	vec2 offset(pos.x - last.x, last.y - pos.y);
	last = pos;

	/* player mouse handler */
	steve->turn(offset);
}

void Rubydung::OnEvent(Event& ev) 
{
	DefaultLayer::OnEvent(ev);

	while (InternalWindow.GetEvent(ev)) {
		Input::ProcessEvent(ev);

		switch (ev.type) {
		case GLEQ_KEY_PRESSED:
			this->OnKeyPressed(ev.keyboard.key);
			break;
		case GLEQ_WINDOW_RESIZED:
			Renderer::Viewport({ 0, 0 }, { ev.size.width, ev.size.height });
			steve->cam.aspect = (float)ev.size.width / (float)ev.size.height;
			break;
		case GLEQ_CURSOR_MOVED:
			OnCursorMoved(ev.pos.x, ev.pos.y);
			break;
		}
		InternalWindow.FreeEvent(ev);
	}
}

void Rubydung::OnRender() 
{
	DefaultLayer::OnRender();

	/* GAME RENDER MAIN PROCESS:
	 * -- Enable chunk shader
	 * -- Render player (get clip matrix view player.cpp)
	 * -- Update player
	 * -- Set fog uniforms in the chunk shader
	 * -- Render level
	 * -- Enable selector shader
	 * -- PlayerPick process
	 */
	sh_chunk->Bind();
	steve->Render(sh_chunk);
	steve->Update();

	sh_chunk->SetVec3("s_cpos", steve->cam.pos);
	sh_chunk->SetVec4("s_fcolor", { 14 / 255.0f, 11 / 255.0f, 10 / 255.0f, 1.0f });
	sh_chunk->SetFloat("s_fstart", -10.0f);
	sh_chunk->SetFloat("s_fend", 20.0f);

	lev->Render(sh_chunk, steve);

	sh_sel->Bind();
	sh_sel->Set4x4("s_VP", steve->cam.GetProj() * steve->cam.GetView());

	PlayerPick();
}

void Rubydung::OnTick() 
{
	DefaultLayer::OnTick();

#if _DEBUG
	RD_INFO << "fps: " << Application::GetInstance().GetFPS() << " ups: " << Application::GetInstance().GetUPS() << " cups: " << lev->GetUpdates();
#else
	printf("%i fps, %i\n", Application::GetInstance().GetFPS(), lev->GetUpdates());
#endif
}

void Rubydung::OnSuspended() {
	DefaultLayer::OnSuspended();
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

		if (blockpos != lblock && lev->IsSolidTile(blockpos)) {
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
	vec3 ray = steve->cam.front;
	vec3 org = steve->cam.pos;

	/* Result of the hit */
	Hitresult ret;

	/* The camera hit solid block? */
	bool hit = Raycast(org, ray, ret);

	if (hit) {
		/* Render player selector */
		py_sel->SetHit(ret);
		py_sel->Render(steve->cam, sh_sel, tm->ElapsedMillis());

		/* Get mouse button status */
		bool left = Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1);
		bool right = Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2);

		/* Avoid click spam */
		if (left && !last_mouse_left)
			lev->SetTile(ret.block, 0); /* delete tile */

		if (right && !last_mouse_right)
			lev->SetTile(ret.block + ret.face, 1); /* set tile */

		/* Update mouse state */
		last_mouse_left = left;
		last_mouse_right = right;
	}
}
	