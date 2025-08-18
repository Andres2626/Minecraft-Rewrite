#include "Rubydung.h"

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
		: Application(WIN_TITLE, { WIN_X, WIN_Y, { false } })
	{ 
	
	}

	~Game()
	{ 
	
	}

	void Init() override
	{
		Application::Init();
		SetFPSGoal(60);

		PushLayer(new Rubydung());
	}
};

int main(int argc, char** argv) 
{
	log_init(MC_LOG_STDOUT | MC_LOG_FILE);
	log_set_level(MC_LEVEL_TRACE);
	Game game;
	game.Start();
	return 0;
}