#include "Minecraft.h"

#ifndef MC_USE_RELEASE
static float rendertime = 0.0f;
static float updatetime = 0.0f;
#endif

static bool mouseswap = 0.0f;

GameProperties GlobalGP;

void DownloadAssets()
{
	WebFileSystem::Init("assets/", "/assets/");
	WebFileSystem::CheckAndDownload("Internal/win_icon.png");
	WebFileSystem::CheckAndDownload("Shaders/gles/chunk.shader");
	WebFileSystem::CheckAndDownload("Shaders/gles/selector.shader");
	WebFileSystem::CheckAndDownload("Shaders/gles/character.shader");
	WebFileSystem::CheckAndDownload("Shaders/gles/hud.shader");
	WebFileSystem::CheckAndDownload("Shaders/gles/particle.shader");
	WebFileSystem::CheckAndDownload("terrain.png");
	WebFileSystem::CheckAndDownload("char.png");
	WebFileSystem::Sync();
}

Minecraft::Minecraft()
	: m_InternalWindow(Application::Get().GetWindow()),
	  m_Props(m_InternalWindow.GetProps())
{
	srand((unsigned int)time(NULL));

	REGISTER_EVENT(EventType::KeyBoardButton, [this](Event& e) { OnEvent(e); });
	REGISTER_EVENT(EventType::WindowResized, [this](Event& e) { OnEvent(e); });
	REGISTER_EVENT(EventType::CursorMoved, [this](Event& e) { OnEvent(e); });

	/* Web: download all assets from localhost without using --preload-file */
	DownloadAssets();

	m_Timer = std::make_unique<Timer>();
	m_WinSize = { m_Props.x, m_Props.y };
    
	/* Set window icon */
	const char *errmsg = {};
	Image wico;
	bool load = wico.LoadFromFile("/Internal/win_icon.png", &errmsg);
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

Minecraft::~Minecraft() 
{

}

bool Minecraft::Init()
{
	Default::Init();

	/* load shaders */
	ShaderManager::Register("chunk", "/Shaders/chunk.shader");
	ShaderManager::Register("selector", "/Shaders/selector.shader");
	ShaderManager::Register("character", "/Shaders/character.shader");
	ShaderManager::Register("hud", "/Shaders/hud.shader");
	ShaderManager::Register("particle", "/Shaders/particle.shader");

	m_Level = std::make_unique<Level>(GlobalGP.LevelSize);
	ModelManager::Init();

	m_Player = std::make_unique<Player>(*m_Level);
	m_EntityManager = std::make_unique<EntityManager>();
	m_ZombieRenderer = std::make_unique<ZombieRenderer>();
	m_ParticleRenderer = std::make_unique<ParticleRenderer>();
	m_Level->SetEntityManager(m_EntityManager.get());
	m_GUI = std::make_unique<gui>(m_Player.get(), m_WinSize);
	m_GUI->Build();
	m_GUI->BuildCrossHair();
	
	for (int i = 0; i < 10; i++) {
		auto zm = std::make_unique<Zombie>(*m_Level, vec3(128.0f, 0.0f, 128.0f));
		zm->ResetPos();
		m_EntityManager->Register<Zombie>(std::move(zm));
	}

	m_ZombieRenderer->SetEntityManager(m_EntityManager.get());
	m_ParticleRenderer->SetEntityManager(m_EntityManager.get());

	/* load texture */
	if (!m_TerrainAtlas.LoadFromFile("/terrain.png", GL_NEAREST)) {
		mc_fatal("failed to open texture file: \"{}\"", m_TerrainAtlas.path);
		return false;
	}

	if (!m_CharAtlas.LoadFromFile("/char.png", GL_NEAREST)) {
		mc_fatal("failed to open char file: \"{}\"", m_CharAtlas.path);
		return false;
	}

	ShaderManager::Get("chunk").SetInt("s_t1", 0);
	ShaderManager::Get("character").SetInt("s_t1", 0);
	ShaderManager::Get("hud").SetInt("s_t1", 0);
	ShaderManager::Get("particle").SetInt("s_t1", 0);

	return true;
}

void Minecraft::OnUpdate(Timestep &ts) 
{
	Default::OnUpdate(ts);
	
	m_Player->UpdateRayCast();
	m_EntityManager->Update();
	m_Player->Update();
	m_Player->Pick();
	m_Level->Update();
}

void Minecraft::OnKey(KeyboardButtonEvent &ev) 
{
	if (ev.action != ButtonAction::Press)
		return; /* at the moment ignore released and repeated actions */

	switch (ev.key) {
	case MC_KEY_G:
		m_EntityManager->Register<Zombie>(std::make_unique<Zombie>(*m_Level, m_Player->attr.pos));
		break;
	case MC_KEY_Y:
		mouseswap = !mouseswap;
		m_Player->SetMouseSwap(mouseswap);
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
	case MC_KEY_6:
		m_Player->SetSelectedBlock(BlockType::BUSH);
		m_GUI->Build();
		break;
	default:
		break;
	}
}

void Minecraft::OnResize(WindowResizeEvent &ev)
{
	m_WinSize = { ev.width, ev.height };
	Renderer::Viewport({ 0, 0 }, m_WinSize);
	m_Player->Cam.aspect = (float)m_WinSize.x / (float)m_WinSize.y;
	m_GUI->OnWindowResize(m_WinSize);
}

void Minecraft::OnCursorMove(CursorMotionEvent &ev)
{
	m_Player->MouseMove({ ev.dx, ev.dy });
}

void Minecraft::OnEvent(Event& ev)
{
	EventType type = static_cast<EventType>(ev.type);
	switch (type) {
	case EventType::KeyBoardButton:
		OnKey(static_cast<KeyboardButtonEvent&>(ev));
		break;
	case EventType::WindowResized:
		OnResize(static_cast<WindowResizeEvent&>(ev));
		break;
	case EventType::CursorMoved:
		OnCursorMove(static_cast<CursorMotionEvent&>(ev));
		break;
    default:
        break;
	}
}

/*
* TODO:
*	- Improve rendering process
*	- Move fog effect to postfx shader.
*/
void Minecraft::OnRender(float alpha)
{
	Default::OnRender(alpha);

	Shader &schunk = ShaderManager::Get("chunk");
	Shader &schar = ShaderManager::Get("character");
	Shader &sparticle = ShaderManager::Get("particle");
	mat4 VP = m_Player->Cam.GetProjection() * m_Player->Cam.GetView();

	/* world rendering */
	schunk.Bind();
	m_TerrainAtlas.Bind(0);
	schunk.Set4x4("s_VP", VP);
	m_Level->Render(m_Player.get());
	m_Player->Render(alpha, m_Timer->ElapsedSeconds());

	/* selector shader */
	m_Player->RenderPick(m_Timer->ElapsedMillis());

	/* particle rendering */
	sparticle.Bind();
	sparticle.Set4x4("s_VP", VP);
	m_ParticleRenderer->Render(*m_Player, alpha);

	/* character rendering */
	schar.Bind();
	m_CharAtlas.Bind(0);
	schar.Set4x4("s_VP", VP);
	m_ZombieRenderer->Render(*m_Level, *m_Player, alpha, m_Timer->ElapsedSeconds());

	/* GUI rendering */
	m_GUI->Render(&m_TerrainAtlas);
}

void Minecraft::OnTick() 
{
	Default::OnTick();

	ChunkManager *cm = m_Level->GetChunkManager();

#ifndef MC_USE_RELEASE
	mc_info("Render time: {:.2f} ms update time {:.2f} ms other: {:.2f}", rendertime, updatetime, 1000 - abs(rendertime - updatetime));
	mc_info("fps: {}, ups: {}, cups: {}, ms/f: {}", Application::Get().GetFPS(), Application::Get().GetUPS(), cm->GetChunkUpdates(), 1000.0f / Application::Get().GetFPS());
	mc_info("Rendered chunks: {} / total chunks: {}", cm->GetDrawCalls(), cm->GetChunksCount());

	rendertime = 0.0f;
	updatetime = 0.0f;
#else
	mc_info("{} fps, {}", Application::Get().GetFPS(), cm->GetChunkUpdates());
#endif

	m_Level->Tick();
}

void Minecraft::OnSuspended() 
{
	Default::OnSuspended();
}

