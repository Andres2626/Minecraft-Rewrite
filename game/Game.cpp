#include "Minecraft.h"

#include <spdlog/spdlog.h>

#include <Log/Log.h>
#include <App/Application.h>
#include <Filesystem/NativeFileSystem.h>
#include <Filesystem/VirtualFileSystem.h>

#define COUNTER_INTERFACE     1
#define TPS                   60.0f
#define WIN_X                 1024
#define WIN_Y                 768
#define WIN_TITLE             "Game"

using namespace MC;
using namespace App;
using namespace File;

class Game : public Application 
{
public:
	Game()
		: Application(WIN_TITLE, { WIN_X, WIN_Y, false, { false }, { CONTEXT_COMPAT_PROFILE, 3, 3, false } })
	{ 
	
	}

	~Game()
	{ 

	}

	bool Init() override
	{
		Application::Init();
		VirtualFileSystem::Mount("/", std::make_unique<NativeFileSystem>("assets/"));
		VirtualFileSystem::Mount("/save", std::make_unique<NativeFileSystem>("./"));

#ifdef MC_PLATFORM_WEB
        VirtualFileSystem::Mount("/Shaders", std::make_unique<NativeFileSystem>("assets/Shaders/gles"));
#else /* !MC_PLATFORM_WEB */
        VirtualFileSystem::Mount("/Shaders", std::make_unique<NativeFileSystem>("assets/Shaders/core"));
#endif /* MC_PLATFORM_WEB */

		PushLayer(new Minecraft());
		return true;
	}
};

int main(int argc, char** argv) 
{
	Game game;
	game.Start();
	return 0;
}