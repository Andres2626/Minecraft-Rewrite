#include "Rubydung.h"

static float rendertime = 0.0f;
static float updatetime = 0.0f;

GameProperties GlobalGP;

Rubydung::Rubydung()
	: m_InternalWindow(Application::Get().GetWindow()),
	  m_Props(m_InternalWindow.GetProps())
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
	Renderer::ClearColor(GlobalGP.SkyColor);
}

Rubydung::~Rubydung() 
{

}

void Rubydung::Init()
{
	Default::Init();

	/* load shaders */
	ShaderManager::Register("chunk", "assets/Shaders/chunk.shader");
	ShaderManager::Register("selector", "assets/Shaders/selector.shader");
	ShaderManager::Register("character", "assets/Shaders/character.shader");
	ShaderManager::Register("hud", "assets/Shaders/hud.shader");

	m_Entities = std::make_unique<EntityManager>();
	m_EntityRenderer = std::make_unique<EntityRenderer>();
	m_Level = std::make_unique<Level>(GlobalGP.LevelSize);
	m_Player = std::make_unique<Player>(*m_Level);
	m_GUI = std::make_unique<gui>(m_Player.get(), m_WinSize);
	m_GUI->Build();
	m_GUI->BuildCrossHair();
	
	for (int i = 0; i < 10; i++) {
		auto zm = std::make_unique<Zombie>(*m_Level, vec3(128.0f, 0.0f, 128.0f));
		zm->ResetPos();
		m_Entities->Register(std::move(zm));
	}

	m_EntityRenderer->SetEntityManager(m_Entities.get());

	/* load texture */
	if (!m_TerrainAtlas.LoadFromFile("assets/terrain.png", GL_NEAREST))
		mc_fatal("failed to open texture file: \"{}\"", m_TerrainAtlas.path);

	if (!m_CharAtlas.LoadFromFile("assets/char.png", GL_NEAREST))
		mc_fatal("failed to open char file: \"{}\"", m_CharAtlas.path);

	ShaderManager::Get("chunk").SetInt("s_t1", 0);
	ShaderManager::Get("character").SetInt("s_t1", 0);
	ShaderManager::Get("hud").SetInt("s_t1", 0);
}

float accumulator = 0.0f;

void Rubydung::OnUpdate(Timestep &ts) 
{
	Default::OnUpdate(ts);
	
	m_Player->UpdateRayCast();	
	m_Entities->Update();
	m_Player->Update();
	m_Player->Pick();
	m_Level->Update();
}

void Rubydung::OnKeyPressed(int key) 
{
	switch (key) {
	case MC_KEY_G:
		m_Entities->Register(std::make_unique<Zombie>(*m_Level, m_Player->attr.pos));
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

	Shader &schunk = ShaderManager::Get("chunk");
	Shader &schar = ShaderManager::Get("character");
	mat4 VP = m_Player->Cam.GetProjection() * m_Player->Cam.GetView();

	/* world rendering */
	schunk.Bind();
	m_TerrainAtlas.Bind(0);
	schunk.Set4x4("s_VP", VP);
	m_Level->Render(m_Player.get());
	m_Player->Render(alpha, m_Timer->ElapsedSeconds());

	/* selector shader */
	m_Player->RenderPick(m_Timer->ElapsedMillis());

	/* character rendering */
	schar.Bind();
	m_CharAtlas.Bind(0);
	schar.Set4x4("s_VP", VP);
	m_EntityRenderer->Render(*m_Level, *m_Player, alpha, m_Timer->ElapsedSeconds());

	/* GUI rendering */
	m_GUI->Render(&m_TerrainAtlas);
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

