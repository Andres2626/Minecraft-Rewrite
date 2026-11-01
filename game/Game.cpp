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
		: Application(WIN_TITLE, { WIN_X, WIN_Y, false })
	{ 
	
	}

	~Game()
	{ 

	}

	ErrorType Init() override
	{
		Application::Init();
		VirtualFileSystem::Mount("/", new NativeFileSystem("assets/"));
		VirtualFileSystem::Mount("/save", new NativeFileSystem("./"));

#ifdef MC_PLATFORM_WEB
        VirtualFileSystem::Mount("/Shaders", new NativeFileSystem("assets/Shaders/gles"));
#else /* !MC_PLATFORM_WEB */
        VirtualFileSystem::Mount("/Shaders", new NativeFileSystem("assets/Shaders/core"));
#endif /* MC_PLATFORM_WEB */

		PushLayer(new Minecraft());
		return ErrorType::NoError;
	}
};

int main(int argc, char** argv) 
{
	Game game;
	game.Start();
	return 0;
}