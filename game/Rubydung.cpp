#include "Rubydung.h"

Rubydung::Rubydung()
	: m_InternalWindow(Application::Get().GetWindow()),
	  m_Props(m_InternalWindow.GetProps()), m_GProperties()
{
	srand((unsigned int)time(NULL));

	m_Timer = std::make_unique<Timer>();
	m_Last = { m_Props.x / 2, m_Props.y / 2 };

	/* Set window icon */
	Image wico;
	bool load = wico.LoadFromFile("assets/Internal/win_icon.png");
	if (!load)
		mc_fatal("failed to open window icon: \"{}\"", wico.path);

	m_InternalWindow.SetIcon(wico);
	wico.Free();

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
	m_CharShader = std::make_unique<Shader>("assets/Shaders/character.shader");
	m_Level = std::make_unique<Level>(m_GProperties.LevelSize);
	m_Player = std::make_unique<Player>(*m_Level);
	
	for (int i = 0; i < 100; i++)
		m_Zombies.push_back(std::make_unique<Zombie>(*m_Level, vec3(128.0f, 0.0f, 128.0f)));


	/* load texture */
	if (!m_TerrainTexture.LoadFromFile("assets/terrain.png", GL_NEAREST))
		mc_fatal("failed to open texture file: \"{}\"", m_TerrainTexture.path);

	if (!m_CharTexture.LoadFromFile("assets/char.png", GL_NEAREST))
		mc_fatal("failed to open char file: \"{}\"", m_CharTexture.path);

	m_CShader->SetInt("t1", 0);
	m_CharShader->SetInt("t1", 0);
}

void Rubydung::OnUpdate(Timestep &ts) 
{
	Default::OnUpdate(ts);
	m_Player->Update();
	m_Player->UpdateRayCast();
	m_Player->Pick();

	for (int i = 0; i < m_Zombies.size(); i++) 
		m_Zombies[i]->Update();
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

void Rubydung::OnCursorMoved(int &x, int &y) 
{
	vec2 offset(x - m_Last.x, m_Last.y - y);
	m_Last = vec2(x, y);
	m_Player->MouseMove(offset);
}

void Rubydung::OnEvent(Event &ev) 
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

void Rubydung::OnRender(float alpha)
{
	Default::OnRender(alpha);
	mat4 VP = m_Player->Cam.GetProj() * m_Player->Cam.GetView();
	
	m_CShader->Bind();
	m_TerrainTexture.Bind();
	m_CShader->Set4x4("s_VP", VP);
	m_CShader->SetVec3("s_cpos", m_Player->Cam.pos);
	m_CShader->SetVec4("s_fcolor", m_GProperties.FogColor);
	m_CShader->SetFloat("s_fstart", m_GProperties.FogStart);
	m_CShader->SetFloat("s_fend", m_GProperties.FogEnd);
	m_CShader->SetInt("t1", 0);
	m_Player->Render();
	m_Level->Render(m_CShader.get(), m_Player.get());
	m_SShader->Bind();
	m_SShader->Set4x4("s_VP", VP);
	m_Player->RenderPick(m_Timer->ElapsedMillis(), m_SShader.get());
	m_CharShader->Bind();
	m_CharTexture.Bind();
	m_CharShader->Set4x4("s_VP", VP);

	for (int i = 0; i < m_Zombies.size(); i++)
		m_Zombies[i]->Render(alpha, m_CharShader.get(), m_Timer->ElapsedSeconds());
}

void Rubydung::OnTick() 
{
	Default::OnTick();

#ifdef MC_USE_RELEASE
	mc_info("fps: {}, ups: {}, cups: {}, ms/f: {}", Application::Get().GetFPS(), Application::Get().GetUPS(), m_Level->GetUpdates(), 1000.0f / Application::Get().GetFPS());
	mc_info("Rendered chunks: {} / total chunks: {}", m_Level->GetDrawCalls(), m_Level->GetChunksCount());
	m_Level->RestartDrawCalls();
#else
	mc_info("{} fps, {}", Application::Get().GetFPS(), m_Level->GetUpdates());
#endif

	m_Level->RestartUpdates();
}

void Rubydung::OnSuspended() 
{
	Default::OnSuspended();
}

