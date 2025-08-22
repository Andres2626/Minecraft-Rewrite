#include "Rubydung.h"

#define CAM_ASPECT (float)m_Props.x / (float)m_Props.y

Rubydung::Rubydung()
	: m_InternalWindow(Application::Get().GetWindow()),
	  m_Props(m_InternalWindow.GetProps()), m_GProperties()
{
	/* initialize random number generator */
	srand(time(NULL));

	m_Timer = std::make_unique<Timer>();
	m_Last = { m_Props.x / 2, m_Props.y / 2 };

	/* Set window icon */
	Image icon;
	bool load = icon.LoadFromFile("assets/Internal/win_icon.png");
	if (!load)
		mc_fatal("failed to open image file: \"%s\"\n", icon.path);
	m_InternalWindow.SetIcon(icon);
	icon.Free();

	Renderer::DepthFunc(DepthValue::LEQUAL);
	Renderer::Enable(GL_DEPTH_TEST);
	Renderer::ClearColor(m_GProperties.SkyColor);
}

Rubydung::~Rubydung() 
{

}

void Rubydung::Init() 
{
	Default::Init();

	/* load shaders */
	m_CShader = std::make_unique<Shader>("assets/Shaders/chunk.shader");
	m_SShader = std::make_unique<Shader>("assets/Shaders/selector.shader");

	/* Create player and level */
	m_Level = std::make_unique<Level>(m_GProperties.LevelSize);
	m_Player = std::make_unique<Player>(*m_Level);

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
		Application::Get().Stop();
	}
}

void Rubydung::OnKeyPressed(int key) 
{
	switch (key) {
	case MC_KEY_ESCAPE:
		m_Level->Save();
		Application::Get().Stop();
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
	vec2 offset(x - m_Last.x, m_Last.y - y);
	m_Last = vec2(x, y);
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
	mat4 VP = m_Player->Cam.GetProj() * m_Player->Cam.GetView();

	/* 
	 * GAME RENDER MAIN PROCESS:
	 * -- Enable chunk shader
	 * -- Render player (Update Camera vectors)
	 * -- Get VP and set it in the chunk shader
	 * -- Update player
	 * -- Set fog uniforms in the chunk shader
	 * -- Render level
	 * -- Enable selector shader
	 * -- PlayerPick process
	 */
	m_CShader->Bind();
	m_Player->Render();

	m_CShader->Set4x4("s_VP", VP);

	m_CShader->SetVec3("s_cpos", m_Player->Cam.pos);
	m_CShader->SetVec4("s_fcolor", m_GProperties.FogColor);
	m_CShader->SetFloat("s_fstart", m_GProperties.FogStart);
	m_CShader->SetFloat("s_fend", m_GProperties.FogEnd);

	m_Level->Render(m_CShader.get(), m_Player.get());

	m_SShader->Bind();
	m_SShader->Set4x4("s_VP", VP);

	m_Player->Pick(m_Timer->ElapsedMillis(), m_SShader.get());
}

void Rubydung::OnTick() 
{
	Default::OnTick();

#ifndef MC_USE_RELEASE
	mc_debug("fps: %i, ups: %i, cups: %i\n", Application::Get().GetFPS(), Application::Get().GetUPS(), m_Level->GetUpdates());
	mc_debug("Rendered chunks: %i / total chunks: %i\n", m_Level->GetDrawCalls(), m_Level->GetChunksCount());
	m_Level->RestartDrawCalls();
#else
	printf("%i fps, %i\n", Application::Get().GetFPS(), m_Level->GetUpdates());
#endif

	m_Level->RestartUpdates();
}

void Rubydung::OnSuspended() 
{
	Default::OnSuspended();
}

