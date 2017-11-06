#include "Engine.h"
#include "Actor.h"
#include "Map.h"
#include "Destructible.h"
#include "Attacker.h"
#include "Ai.h"
#include "Gui.h"
#include "Container.h"
#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS/Systems/RenderSystem.h"



Engine::Engine(int screenWidth, int screenHeight) : gameStatus(STARTUP), fovRadius(10), screenWidth(screenWidth), screenHeight(screenHeight)
{
	TCODConsole::initRoot(screenWidth, screenHeight, "C++ libtcod tutorial", false);

	// Create camera
	camera = world.createEntity();
	Coordinates cop = {60, 60, 9};
	camera.addComponent<PositionComponent>(cop);
	camera.addComponent<RenderComponent>('&', TCODColor::white, TCODColor::black);
	camera.activate();

	// Add systems at boot -> move all these things to local map once made
	renderSystem.mCameraPos = &camera.getComponent<PositionComponent>().co;											// Note to self, create a tile manager class. Replace tile variables with a U16/U32 bitmap of info for simple &'s
																													// Another idea, use some bit's to index a data struct type in an array to get more out of bit's say lsb is data index for granite, etc!


	world.addSystem(renderSystem);

	world.refresh();

	player = new Actor({ 40, 25, 1 }, '@', PLAYER_ID, "player", TCODColor::white);					 // Final x, y are determined in map! CHANGE THIS CONSTRUCTORS Z LEVEL LATER?
	player->destructible = new PlayerDestructible(30, 2, "Your cadaver!");
	player->attacker = new Attacker(5);
	player->ai = new PlayerAi();
	player->container = new Container(26);

	actors.push(player);
	map = new Map(screenWidth, screenHeight, MAX_ZLVL);
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
	//camera->update();
	//camera->draw();
	renderSystem.update();

	// Iteratre through actors, setting chars location and colors
	for (Actor * actor : actors) 
	{
		if(map->isInFov(actor->co.x, actor->co.y))
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
