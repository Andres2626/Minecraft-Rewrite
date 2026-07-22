#include "Rubydung.h"

#include <spdlog/spdlog.h>

#include <Log/Log.h>
#include <App/Application.h>

#define COUNTER_INTERFACE     1
#define TPS                   60.0f
#define WIN_X                 1024
#define WIN_Y                 768
#define WIN_TITLE             "Game"

using namespace MC;
using namespace App;

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

	void Init() override
	{
		Application::Init();
		PushLayer(new Rubydung());
	}
};

int main(int argc, char** argv) 
{
	Game game;
	game.Start();
	return 0;
}