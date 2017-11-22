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
#include "ECS\Systems\MovementAiSystem.h"
#include "ECS\Systems\JobsSystem.h"
#include "BearLibTerminal.h"

#include <chrono>
typedef std::chrono::milliseconds::rep TimePoint;
inline TimePoint now() {
	return std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::steady_clock::now().time_since_epoch()).count();
}


Engine::Engine(int screenWidth, int screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight)
{
	// Create local map
	map = new Map(screenWidth, screenHeight, MAX_ZLVL);

	// Add systems at boot -> move all these things to local map once made ?
	renderSystem = new RenderSystem(map);
	movementSystem = new MovementSystem();
	movementAiSystem = new MovementAiSystem(&map->tileManager);
	jobsSystem = new JobsSystem();

	world.addSystem(*renderSystem);
	world.addSystem(*movementSystem);
	world.addSystem(*movementAiSystem);
	world.addSystem(*jobsSystem);

	// FloodFill from 0, 0, MAX_Z_LVL - 1
	// explored areas. Not working yet.
	//movementAiSystem->floodFillMap();

	world.refresh();
}


Engine::~Engine()
{
	delete map;
	delete renderSystem;
}

// Game loop
void Engine::run()
{
	/*
	double t = 0.0;
	static const double dt = 1.0 / 60.0;

	double currentTime = now();

	while (true)
	{

		double newTime = now();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		// This will have issues when the game becomes simulation bound!@!@!@!@!@!
		while (frameTime > 0.0)
		{
			double deltaTime = std::min(frameTime, dt);
			update(deltaTime);

			frameTime -= deltaTime;
			t += deltaTime;
		}	
		render();
	}
	*/
	// This loop vs above??

	const double MS_PER_UPDATE = 10.0;
	double previous = now();
	double lag = 0.0;

	while (true)
	{
		double current = now();
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

	// Update systems
	movementAiSystem->update();
	movementSystem->update(deltaTime);
	jobsSystem->update(deltaTime);

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
