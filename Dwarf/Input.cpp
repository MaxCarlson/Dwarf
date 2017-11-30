#include "Input.h"

#include "Engine.h"
#include "Map.h"
#include "MapRender.h"

#include "BearLibTerminal.h"
#include "ECS\Systems\JobsSystem.h"
#include "Designations.h"

#include "Tile.h"

Input::Input()
{
}


Input::~Input()
{
}

void Input::read()
{
	int keyPress;

	// Don't block engine waiting for input
	if (terminal_peek())
		keyPress = terminal_read();
	else
		return;

	switch (keyPress)
	{
// Camera
	// Move camera up or down z Levels
	case TK_COMMA:
		engine.map->mapRenderer->incrementZLevel(-1);
		break;

	case TK_PERIOD:
		engine.map->mapRenderer->incrementZLevel(1);
		break;

	// Directional movement for camera
	case TK_UP:
		engine.map->mapRenderer->moveCamera(MapRender::NORTH);
		break;

	case TK_DOWN:
		engine.map->mapRenderer->moveCamera(MapRender::SOUTH);
		break;

	case TK_RIGHT:
		engine.map->mapRenderer->moveCamera(MapRender::EAST);
		break;

	case TK_LEFT:
		engine.map->mapRenderer->moveCamera(MapRender::WEST);
		break;


// Test
	case TK_MOUSE_LEFT:
		int xx = terminal_state(TK_MOUSE_X);
		int yy = terminal_state(TK_MOUSE_Y);

		//Job(Coordinates co, int exp, int bSkill, double duration, Job::Jobs jobType)
		//	: co(co), experience(exp), baseSkillReq(bSkill), baseDuration(duration), jobType(jobType) {}

		//designations->mining.emplace(Coordinates{ xx, yy, engine.map->mapRenderer->currentZLevel }, 1);

		Coordinates co = { xx, yy, engine.map->mapRenderer->currentZLevel };

		designations->mining.emplace(TILE_ARRAY_LOOKUP, 1);

		//Job j({ xx, yy, engine.map->mapRenderer->currentZLevel }, 10, 1, 14, Job::MINER);

		//engine.jobsSystem->addJob(j);
		break;
	

	//default: break;
	}
}
