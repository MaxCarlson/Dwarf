#include "stdafx.h"
#include "PlayGameLoop.h"
#include "ECS\Systems\MiningSystem.h"
#include "Globals\GlobalWorld.h"
namespace Details
{
	bool init = true;


}

std::unordered_map<std::string, SystemBase*> systems;

void PlayGameLoop::run(const double duration)
{
	using namespace Details;

	const std::string apple = "all";

	systems[apple] = new MiningSystem();

	world.addSystem(*systems[apple]);

	world.refresh();

	int a = 5;
}


