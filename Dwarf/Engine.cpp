#include "Engine.h"

#include "Map.h"
#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\Components\KeyBoardComponent.h"
#include "ECS\Components\CameraComponent.h"
#include "ECS\Systems\RenderSystem.h"
#include "ECS\Systems\CameraSystem.h"
#include "BearLibTerminal.h"



Engine::Engine(int screenWidth, int screenHeight) : gameStatus(STARTUP), fovRadius(10), screenWidth(screenWidth), screenHeight(screenHeight)
{
	//TCODConsole::setCustomFont("../Obsidian_16x16.png");
	TCODConsole::initRoot(screenWidth, screenHeight, "Dwarf", false); // Why so big with custom font?

	// Create camera
	camera = world.createEntity();
	Coordinates cop = {60, 60, 9};
	camera.addComponent<PositionComponent>(cop);
	camera.addComponent<RenderComponent>(16, TCODColor::white, TCODColor::azure);
	camera.addComponent<KeyboardComponent>();
	camera.getComponent<KeyboardComponent>().lastKeyPressed = &lastKey;
	camera.addComponent<CameraComponent>(screenWidth, screenHeight);
	camera.activate();

	// Add systems at boot -> move all these things to local map once made
	renderSystem = new RenderSystem(&camera.getComponent<PositionComponent>().co, &camera.getComponent<CameraComponent>());
																													
	cameraSystem = new CameraSystem();

	world.addSystem(*renderSystem);
	world.addSystem(*cameraSystem);

	world.refresh();

	map = new Map(screenWidth, screenHeight, MAX_ZLVL);
}


Engine::~Engine()
{
	delete map;
	delete renderSystem;
	delete cameraSystem;
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

	//terminal_put(50, 10, 0xE200 + 2);
	//terminal_printf(60, 10, "[font=tile]%i", 0xE100 + 80);
	//terminal_put_ext(50, 20, 16, 16, 'U'+20+ 'AC');
	

	map->render();

	// Update systems
	cameraSystem->update();
	renderSystem->update();
}
