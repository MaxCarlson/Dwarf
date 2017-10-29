#include "Engine.h"
#include "Actor.h"
#include "Map.h"


Engine::Engine() : gameStatus(STARTUP), fovRadius(10)
{
	TCODConsole::initRoot(80, 50, "C++ libtcod tutorial", false);
	player = new Actor(40, 25, '@', "player", TCODColor::white);
	actors.push(player);
	map = new Map(80, 45);
}


Engine::~Engine()
{
	actors.clearAndDelete();
	delete map;
}

void Engine::update()
{
	TCOD_key_t key;
	if (gameStatus == STARTUP)
		map->computeFov();
	gameStatus = IDLE;

	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);

	int dx = 0, dy = 0;
	switch (key.vk)
	{
		
	case TCODK_UP:    dy = -1; break;
	case TCODK_DOWN:  dy =  1; break;
	case TCODK_RIGHT: dx =  1; break;
	case TCODK_LEFT:  dx = -1; break;

	default: break;		
	}

	if (dx != 0 || dy != 0) {
		gameStatus = NEW_TURN;
		if (player->moveOrAttack(player->x + dx, player->y + dy))
			map->computeFov();
	}

	if (gameStatus == NEW_TURN)
		for (Actor * actor : engine.actors)
			if (actor != player)
				actor->update();		
}

void Engine::render()
{
	TCODConsole::root->clear();

	map->render();

	// Iteratre through actors, setting chars location and colors
	for (Actor * actor : actors) 
	{
		if(map->isInFov(actor->x, actor->y))
			actor->render();
	}
}
