#include "Engine.h"

#include "Map/Map.h"
#include "Map/MapRender.h"
#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\Components\KeyBoardComponent.h"
#include "ECS\Components\CameraComponent.h"
#include "ECS\Systems\RenderSystem.h"
#include "ECS\Systems\CameraSystem.h"
#include "ECS\Systems\MovementSystem.h"
#include "ECS\Systems\ai\MovementAiSystem.h"
#include "ECS\Systems\MiningSystem.h"
#include "ECS\Systems\DijkstraSystems\DijkstraMapsHandler.h"
#include "ECS\Systems\EntityPositionCache.h"
#include "ECS\Systems\ai\EquipHandler.h"
#include "BearLibTerminal.h"
#include "Designations.h"
#include "ECS\Systems\ai\AiWorkSystem.h"
#include "ECS\Systems\ai\MiningAi.h"
#include "Coordinates.h"
#include <unordered_map>
#include <chrono>



typedef std::chrono::milliseconds::rep TimePoint;
inline TimePoint now() {
	return std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::steady_clock::now().time_since_epoch()).count();
}

bool Game_Paused = false;

Engine::~Engine()
{
	delete map;
	//delete renderSystem;
	//delete movementSystem;
	//delete movementAiSystem;
	//delete miningSystem;
	//delete aiWorkSystem;
	//delete miningAi;
	//delete dijkstraHandler;
	//delete entityPositionCache;
	//delete equipHandler;

	// Some globals
	delete designations;
}

void Engine::init(std::string mapPath, int screenWidth, int screenHeight)
{
	// Create local map
	if (!mapPath.size())
		map = new Map(screenWidth, screenHeight, MAX_ZLVL);
	else
		loadMap(mapPath);

	// Init misc maps and designations
	designations = new Designations;

	// Add systems at init
	renderSystem = new RenderSystem(map);
	movementSystem = new MovementSystem();
	movementAiSystem = new MovementAiSystem();
	movementAiSystem->init();
	aiWorkSystem = new AiWorkSystem();
	miningAi = new MiningAi();
	miningAi->init();

	// Non Entity or non Updating Systems
	miningSystem = new MiningSystem();
	dijkstraHandler = new DijkstraMapsHandler();
	entityPositionCache = new EntityPositionCache();
	equipHandler = new EquipHandler();


	// Introduce systems to the world
	world.addSystem(*renderSystem);
	world.addSystem(*movementSystem);
	world.addSystem(*movementAiSystem);
	world.addSystem(*miningSystem);
	world.addSystem(*aiWorkSystem);
	world.addSystem(*miningAi);
	world.addSystem(*dijkstraHandler);
	world.addSystem(*entityPositionCache);
	world.addSystem(*equipHandler);

	// Init systems with messages
	miningSystem->init();
	dijkstraHandler->init();
	entityPositionCache->init();
	equipHandler->init();

	// FloodFill from 0, 0, MAX_Z_LVL - 1
	// explored areas. Not working yet.
	//movementAiSystem->floodFillMap();
	
	world.refresh();
}

void Engine::loadMap(std::string filePath)
{

}

// Game loop
void Engine::run()
{
	const double MS_PER_UPDATE = 15.0;
	double previous = double(now());
	double lag = 0.0;

	while (true)
	{
		double current = double(now());
		double elapsed = current - previous;
		previous = current;
		lag += elapsed;

		input.read();

		if (!Game_Paused)
		{
			while (lag >= MS_PER_UPDATE)
			{
				update(MS_PER_UPDATE);
				lag -= MS_PER_UPDATE;
			}
		}

		else
			lag = 0.0;

		render();

		if (current_game_state == Engine::TO_MAIN_MENU)
		{
			terminal_clear();
			return;
		}
			
	}
}

void Engine::update(double deltaTime)
{
	// Deliver any deffered messages to systems
	world.deliver_messages();

	// Update systems
	movementAiSystem->update();
	movementSystem->update(deltaTime);
	dijkstraHandler->update();

	// Work Systems
	aiWorkSystem->update();
	miningAi->update(deltaTime);

	// Systems that don't need updates
	// miningSystem->update();
	// entityPositionCache->update();

	// Should this be called before or after? Probably before?
	// Possibly right after deliver messages, so systems can work on updated entities?
	// Figure it out!
	world.refresh();
}

void Engine::render()
{	
	// Clear terminal before render
	terminal_clear();

	// Render the local map
	map->mapRenderer->render();

	// Render Entities
	renderSystem->update();

	// Render the gui elements last,
	// just in case something is showing through
	// that shouldn't
	gui.render();

	// Refresh terminal window to display
	// render changes
	terminal_refresh();
}
