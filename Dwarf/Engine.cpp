#include "Engine.h"
#include "Actor.h"
#include "Map.h"
#include "Destructible.h"
#include "Attacker.h"
#include "Ai.h"
#include "Gui.h"
#include "Container.h"

Engine::Engine(int screenWidth, int screenHeight) : gameStatus(STARTUP), fovRadius(10), screenWidth(screenWidth), screenHeight(screenHeight)
{
	TCODConsole::initRoot(screenWidth, screenHeight, "C++ libtcod tutorial", false);
	player = new Actor(40, 25, '@', "player", TCODColor::white);
	player->destructible = new PlayerDestructible(30, 2, "Your cadaver!");
	player->attacker = new Attacker(5);
	player->ai = new PlayerAi();
	player->container = new Container(26);

	actors.push(player);
	map = new Map(screenWidth, screenHeight);
	gui = new Gui();

	gui->message(TCODColor::azure, "Welcome!");
}


Engine::~Engine()
{
	actors.clearAndDelete();
	delete map;
	delete gui;
}

void Engine::update()
{
	if (gameStatus == STARTUP)
		map->computeFov();

	gameStatus = IDLE;

	// Check for keyboard or mouse input
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &lastKey, &mouse);

	player->update();

	if (gameStatus == NEW_TURN)
		for (Actor * actor : engine.actors)
			if (actor != player)
				actor->update();

	player->render();
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

	gui->render();
}

// Render dead actors first so we see living ones if they're on top
void Engine::sendToBack(Actor * actor)
{
	actors.remove(actor);
	actors.insertBefore(actor, 0);
}
