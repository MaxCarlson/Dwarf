#include "Engine.h"

#include "Map.h"
#include "MapRender.h"
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
	//TCODConsole::initRoot(screenWidth, screenHeight, "Dwarf", false); 

	camera = factory.createCamera(screenWidth, screenHeight);

	// Create local map
	map = new Map(screenWidth, screenHeight, MAX_ZLVL);

	// Add systems at boot -> move all these things to local map once made
	renderSystem = new RenderSystem(map);
																													
	cameraSystem = new CameraSystem();

	world.addSystem(*renderSystem);
	world.addSystem(*cameraSystem);

	world.refresh();
}


Engine::~Engine()
{
	delete map;
	delete renderSystem;
	delete cameraSystem;
}

void Engine::update()
{
	// Update systems
	cameraSystem->update();

	// Should this be called before or after? Probably before?
	world.refresh();
}

void Engine::render()
{
	//TCODConsole::root->clear();

	//terminal_put(50, 10, 0xE200 + 2);
	//terminal_printf(60, 10, "[font=tile]%i", 0xE100 + 80);
	//terminal_put_ext(50, 20, 16, 16, 'U'+20+ 'AC');
	

	map->mapRenderer->render();

	// Should this be in Engine::render()?
	// Should definitely be the clost to the last thing we do
	renderSystem->update();
}
