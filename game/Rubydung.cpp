#include "Rubydung.h"

static float rendertime = 0.0f;
static float updatetime = 0.0f;

Rubydung::Rubydung()
	: m_InternalWindow(Application::Get().GetWindow()),
	  m_Props(m_InternalWindow.GetProps()), m_GProperties()
{
	srand((unsigned int)time(NULL));

	m_Timer = std::make_unique<Timer>();
	m_WinSize = { m_Props.x, m_Props.y };
	m_Last = { m_Props.x / 2, m_Props.y / 2 };

	/* Set window icon */
	const char *errmsg = {};
	Image wico;
	bool load = wico.LoadFromFile("assets/Internal/win_icon.png", errmsg);
	if (!load) {
		mc_error("error loading image. internal error {}", errmsg);
		mc_fatal("failed to open window icon: \"{}\"", wico.path);
	}

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
	m_GUIShader = std::make_unique<Shader>("assets/Shaders/hud.shader");
	m_Level = std::make_unique<Level>(m_GProperties.LevelSize);

	m_Player = std::make_unique<Player>(*m_Level);
	m_GUI = std::make_unique<gui>(m_Player.get(), m_WinSize);
	m_GUI->Build();
	m_GUI->BuildCrossHair();
	
	for (int i = 0; i < 10; i++) {
		auto zm = std::make_unique<Zombie>(*m_Level, vec3(128.0f, 0.0f, 128.0f));
		zm->ResetPos();
		m_Entities.Register(std::move(zm));
	}

	m_EntityRenderer.SetEntityManager(&m_Entities);

	/* load texture */
	if (!m_TerrainAtlas.LoadFromFile("assets/terrain.png", GL_NEAREST))
		mc_fatal("failed to open texture file: \"{}\"", m_TerrainAtlas.path);

	if (!m_CharAtlas.LoadFromFile("assets/char.png", GL_NEAREST))
		mc_fatal("failed to open char file: \"{}\"", m_CharAtlas.path);

	m_CShader->SetInt("s_t1", 0);
	m_CharShader->SetInt("s_t1", 0);
	m_GUIShader->SetInt("s_t1", 0);
}

float accumulator = 0.0f;

void Rubydung::OnUpdate(Timestep &ts) 
{
	Default::OnUpdate(ts);
	
	m_Player->UpdateRayCast();	
	m_Entities.Update();
	m_Player->Update();
	m_Player->Pick();
	m_Level->Update();
}

void Rubydung::OnKeyPressed(int key) 
{
	switch (key) {
	case MC_KEY_G:
		m_Entities.Register(std::make_unique<Zombie>(*m_Level, m_Player->attr.pos));
		break;
	case MC_KEY_ESCAPE:
		m_Level->Save();
		Application::Get().Stop();
		break;
	case MC_KEY_ENTER:
		m_Level->Save();
		break;
	case MC_KEY_1:
		m_Player->SetSelectedBlock(BlockType::ROCK);
		m_GUI->Build();
		break;
	case MC_KEY_2:
		m_Player->SetSelectedBlock(BlockType::DIRT);
		m_GUI->Build();
		break;
	case MC_KEY_3:
		m_Player->SetSelectedBlock(BlockType::STONEBRICK);
		m_GUI->Build();
		break;
	case MC_KEY_4:
		m_Player->SetSelectedBlock(BlockType::WOOD);
		m_GUI->Build();
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
			OnKeyPressed(ev.keyboard.key);
			break;
		case GLEQ_WINDOW_RESIZED:
			m_WinSize = { ev.size.width, ev.size.height };
			Renderer::Viewport({ 0, 0 }, m_WinSize);
			m_Player->Cam.aspect = (float)m_WinSize.x / (float)m_WinSize.y;
			m_GUI->OnWindowResize(m_WinSize);
			break;
		case GLEQ_CURSOR_MOVED:
			OnCursorMoved(ev.pos.x, ev.pos.y);
			break;
		}
		m_InternalWindow.FreeEvent(ev);
	}
}

/*
* TODO:
*	- Improve rendering process
*	- Move fog effect to postfx shader.
*/
void Rubydung::OnRender(float alpha)
{
	Default::OnRender(alpha);
	mat4 VP = m_Player->Cam.GetProjection() * m_Player->Cam.GetView();

	/* world rendering */
	m_CShader->Bind();
	m_TerrainAtlas.Bind(0);
	m_CShader->Set4x4("s_VP", VP);
	m_CShader->SetVec3("s_cpos", m_Player->Cam.pos);
	m_CShader->SetVec4("s_fcolor0", m_GProperties.fg0.color);
	m_CShader->SetFloat("s_fdensity0", m_GProperties.fg0.density);
	m_CShader->SetVec4("s_fcolor1", m_GProperties.fg1.color);
	m_CShader->SetFloat("s_fdensity1", m_GProperties.fg1.density);
	m_Player->Render(m_CShader.get(), alpha, m_Timer->ElapsedSeconds());
	m_Level->Render(m_CShader.get(), m_Player.get());

	/* selector shader */
	m_SShader->Bind();
	m_SShader->Set4x4("s_VP", VP);
	m_Player->RenderPick(m_Timer->ElapsedMillis(), m_SShader.get());
	m_CharShader->Bind();
	m_CharAtlas.Bind(0);

	/* character rendering */
	m_CharShader->Set4x4("s_VP", VP);
	m_CharShader->SetVec3("s_cpos", m_Player->Cam.pos);
	m_CharShader->SetVec4("s_fcolor0", m_GProperties.fg0.color);
	m_CharShader->SetFloat("s_fdensity0", m_GProperties.fg0.density);
	m_CharShader->SetVec4("s_fcolor1", m_GProperties.fg1.color);
	m_CharShader->SetFloat("s_fdensity1", m_GProperties.fg1.density);
	m_EntityRenderer.Render(*m_Level, m_Player->Cam, m_CharShader.get(), alpha, m_Timer->ElapsedSeconds());

	/* GUI rendering */
	m_GUI->Render(m_GUIShader.get(), &m_TerrainAtlas);
}

void Rubydung::OnTick() 
{
	Default::OnTick();

	ChunkManager *cm = m_Level->GetChunkManager();

#ifndef MC_USE_RELEASE
	mc_info("Render time: {:.2f} ms update time {:.2f} ms other: {:.2f}", rendertime, updatetime, 1000 - abs(rendertime - updatetime));
	mc_info("fps: {}, ups: {}, cups: {}, ms/f: {}", Application::Get().GetFPS(), Application::Get().GetUPS(), cm->GetChunkUpdates(), 1000.0f / Application::Get().GetFPS());
	mc_info("Rendered chunks: {} / total chunks: {}", cm->GetDrawCalls(), cm->GetChunksCount());
#else
	mc_info("{} fps, {}", Application::Get().GetFPS(), cm->GetChunkUpdates());
#endif

	rendertime = 0.0f;
	updatetime = 0.0f;

	m_Level->Tick();
}

void Rubydung::OnSuspended() 
{
	Default::OnSuspended();
}

