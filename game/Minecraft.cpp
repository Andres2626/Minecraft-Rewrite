#include "Minecraft.h"

#include <Utils/Profiler.h>

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
	WebFileSystem::CheckAndDownload("Shaders/gles/hud2.shader");
	WebFileSystem::CheckAndDownload("Shaders/gles/particle.shader");
	WebFileSystem::CheckAndDownload("terrain.png");
	WebFileSystem::CheckAndDownload("char.png");
    WebFileSystem::CheckAndDownload("default.gif");
	WebFileSystem::Sync();
}

Minecraft::Minecraft()
	: m_InternalWindow(Application::GetContext().Win),
	  m_Props(m_InternalWindow->GetProperties())
{
	PROFILE_FUNCTION();
	srand((unsigned int)time(NULL));

	REGISTER_EVENT(EventType::KeyBoardButton, this);
	REGISTER_EVENT(EventType::WindowResized, this);
	REGISTER_EVENT(EventType::CursorMoved, this);

	/* Web: download all assets from localhost without using --preload-file */
	DownloadAssets();

	m_Timer = std::make_unique<Timer>();
	m_WinSize = { m_Props.width, m_Props.height };
    
	/* Set window icon */
	const char *errmsg = {};
	Image wico;
	bool load = wico.LoadFromFile("/Internal/win_icon.png", &errmsg);
	if (!load) {
		mc_error("error loading image. internal error {}", errmsg);
		mc_fatal("failed to open window icon: \"{}\"", wico.path);
	}
	m_InternalWindow->SetIcon(wico);

	Input::SetCursorState(m_InternalWindow->GetHandle(), MC_CURSOR_DISABLED);

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
	PROFILE_FUNCTION();

	Default::Init();

	/* load shaders */
	ShaderManager::Register("chunk", "/Shaders/chunk.shader");
	ShaderManager::Register("selector", "/Shaders/selector.shader");
	ShaderManager::Register("character", "/Shaders/character.shader");
	ShaderManager::Register("hud", "/Shaders/hud.shader");
	ShaderManager::Register("hud2", "/Shaders/hud2.shader");
	ShaderManager::Register("particle", "/Shaders/particle.shader");

	BlockManager::Init();
	ModelManager::Init();
	ChunkMesh::Init();

	m_Level = std::make_unique<Level>(GlobalGP.LevelSize);
	m_Player = std::make_unique<Player>(*m_Level);
	m_EntityManager = std::make_unique<EntityManager>();
	m_ZombieRenderer = std::make_unique<ZombieRenderer>();
	m_ParticleRenderer = std::make_unique<ParticleRenderer>();
	m_Level->SetEntityManager(m_EntityManager.get());
	m_GUI = std::make_unique<gui>(m_Player.get(), m_WinSize);

	for (int i = 0; i < 10; i++) {
		auto zm = std::make_unique<Zombie>(*m_Level, vec3(128.0f, 0.0f, 128.0f));
		zm->ResetPos();
		m_EntityManager->Register<Zombie>(std::move(zm));
	}

	m_ZombieRenderer->SetEntityManager(m_EntityManager.get());
	m_ParticleRenderer->SetEntityManager(m_EntityManager.get());

	/* load textures */
	if (!m_CharAtlas.LoadFromFile("/char.png", GL_NEAREST)) {
		mc_fatal("failed to open char file: \"{}\"", m_CharAtlas.path);
		return false;
	}

	m_CharAtlas.Free();
	return true;
}

void Minecraft::OnUpdate(Timestep &ts) 
{
	PROFILE_FUNCTION();
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
	PROFILE_FUNCTION();
	m_WinSize = { ev.width, ev.height };
	Renderer::Viewport({ 0, 0 }, m_WinSize);
	m_Player->Cam.aspect = (float)m_WinSize.x / (float)m_WinSize.y;
}

void Minecraft::OnCursorMove(CursorMotionEvent &ev)
{
	PROFILE_FUNCTION();
	m_Player->MouseMove({ ev.dx, ev.dy });
}

void Minecraft::OnEvent(Event& ev)
{
	PROFILE_FUNCTION();
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
	PROFILE_FUNCTION();
	Default::OnRender(alpha);
	m_Player->UpdateCamera(alpha, m_Timer->ElapsedSeconds());
	m_Level->Render(m_Player.get());
	m_Player->RenderPick(m_Timer->ElapsedMillis());
	m_ParticleRenderer->Render(*m_Player, alpha);
	m_ZombieRenderer->Render(*m_Level, m_CharAtlas, *m_Player, alpha, m_Timer->ElapsedSeconds());
	m_GUI->Render(&m_Level->GetTerrainAtlas());
}

void Minecraft::OnTick() 
{
	Default::OnTick();
	ApplicationContext &ctx = Application::Get().GetContext();
	ChunkManager *cm = m_Level->GetChunkManager();
	m_GUI->OnTick(ctx.FPS, cm->GetChunkUpdates());
	

#if defined(MC_ENABLE_PROFILER)
	mc_trace("====================PROFILER RESUTLS====================");
	mc_trace("FPS: {}  UPS: {}  ms per frame: {}", ctx.FPS, ctx.UPS, 1000.0f / ctx.FPS);
	mc_trace("Chunk Updates: {}", cm->GetChunkUpdates());
	mc_trace("Chunk Draws: {} Total Chunks: {}", cm->GetDrawCalls(), cm->GetChunksCount());
	mc_trace("--------------------------------------------------------");
	ProfilerManager::Get().OnTick();
	mc_trace("========================================================");
#endif

	m_Level->Tick();
}

void Minecraft::OnSuspended() 
{
	Default::OnSuspended();
}

