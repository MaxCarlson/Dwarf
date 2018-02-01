#include "stdafx.h"
#include "PlayGameLoop.h"
#include "Globals\game_states.h"
#include "Globals\GlobalWorld.h"
#include "RunSystems.h"
#include "RegisterComponents.h"

namespace Details
{

}

std::unordered_map<std::string, SystemBase*> systems;

void PlayGameLoop::run(const double duration)
{
	//using namespace Details;

	if (gameState == GameState::NEW_GAME)
	{
		RunSystems::initSystems();
		ComponentsInit::init(); // This isn't strictly neccasary if a new game has been started before, probably won't cause issues but keep a look out!
	}


	RunSystems::updateSystems(duration);
}


