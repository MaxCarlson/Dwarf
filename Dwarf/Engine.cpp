#include "Engine.h"

#include "Map.h"
#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\Components\KeyBoardComponent.h"
#include "ECS\Systems\RenderSystem.h"
#include "ECS\Systems\CameraSystem.h"



Engine::Engine(int screenWidth, int screenHeight) : gameStatus(STARTUP), fovRadius(10), screenWidth(screenWidth), screenHeight(screenHeight)
{
	TCODConsole::initRoot(screenWidth, screenHeight, "C++ libtcod tutorial", false);

	// Create camera
	camera = world.createEntity();
	Coordinates cop = {60, 60, 9};
	camera.addComponent<PositionComponent>(cop);
	camera.addComponent<RenderComponent>('&', TCODColor::white, TCODColor::black);
	camera.addComponent<KeyboardComponent>();
	camera.getComponent<KeyboardComponent>().lastKeyPressed = &lastKey;
	camera.activate();

	renderSystem = new RenderSystem();
	// Add systems at boot -> move all these things to local map once made
	renderSystem->mCameraPos = &camera.getComponent<PositionComponent>().co;											
																													
	cameraSystem = new CameraSystem();

	world.addSystem(*renderSystem);
	world.addSystem(*cameraSystem);

	world.refresh();

	map = new Map(129, 129, MAX_ZLVL);
}


Engine::~Engine()
{
	actors.clearAndDelete();
	delete map;
	delete gui;
}

void Engine::update()
{

	// Check for keyboard or mouse input
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &lastKey, &mouse);

	// Should this be called before or after? Probably before?
	world.refresh();
}

void Engine::render()
{
	TCODConsole::root->clear();

	map->render();

	// Update systems
	cameraSystem->update();
	renderSystem->update();
}

// Render dead actors first so we see living ones if they're on top
void Engine::sendToBack(Actor * actor)
{
	actors.remove(actor);
	actors.insertBefore(actor, 0);
}
