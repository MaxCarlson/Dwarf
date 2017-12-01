#include "Engine.h"

#include "Map.h"
#include "MapRender.h"
#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\Components\KeyBoardComponent.h"
#include "ECS\Components\CameraComponent.h"
#include "ECS\Systems\RenderSystem.h"
#include "ECS\Systems\CameraSystem.h"
#include "ECS\Systems\MovementSystem.h"
#include "ECS\Systems\ai\MovementAiSystem.h"
#include "ECS\Systems\MiningSystem.h"
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

Engine::~Engine()
{
	delete map;
	delete renderSystem;
}

void Engine::init(int screenWidth, int screenHeight)
{
	// Create local map
	map = new Map(screenWidth, screenHeight, MAX_ZLVL);

	// Init misc maps and designations
	designations = new Designations;

	// Add systems at init
	renderSystem = new RenderSystem(map);
	movementSystem = new MovementSystem();
	movementAiSystem = new MovementAiSystem(&map->tileManager);

	// Non Entity Systems
	miningSystem = new MiningSystem(&map->tileManager);

	aiWorkSystem = new AiWorkSystem();
	miningAi = new MiningAi(&map->tileManager);
	miningAi->init();

	// Introduce systems to the world
	world.addSystem(*renderSystem);
	world.addSystem(*movementSystem);
	world.addSystem(*movementAiSystem);
	world.addSystem(*miningSystem);
	world.addSystem(*aiWorkSystem);
	world.addSystem(*miningAi);

	// Init systems with messages
	miningSystem->init();

	// FloodFill from 0, 0, MAX_Z_LVL - 1
	// explored areas. Not working yet.
	//movementAiSystem->floodFillMap();
	
	world.refresh();
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

		// Read user input
		input.read();

		while (lag >= MS_PER_UPDATE)
		{
			update(MS_PER_UPDATE);
			lag -= MS_PER_UPDATE;
		}

		render();
	}
}

void Engine::update(double deltaTime)
{
	// Deliver any deffered messages to systems
	world.deliver_messages();

	// Update systems
	movementAiSystem->update();
	movementSystem->update(deltaTime);
	miningSystem->update();

	// Work Systems
	aiWorkSystem->update();
	miningAi->update(deltaTime);

	// Should this be called before or after? Probably before?
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
