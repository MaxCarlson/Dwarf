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
		
	// Camera system is used for camera controls atm
	// all rendering tricks with camera are done in MapRender
	// and RenderSystem
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
	// Render the local map
	map->mapRenderer->render();

	// Should this be in Engine::render()?
	renderSystem->update();

	// Render the gui elements last,
	// just in case something is showing through
	// that shouldn't
	gui.render();
}
