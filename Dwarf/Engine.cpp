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

	// Add systems at boot -> move all these things to local map once made
	renderSystem = new RenderSystem(map);

	world.addSystem(*renderSystem);

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
	
	double t = 0.0;
	static const double dt = 1.0 / 60.0;

	double currentTime = now();

	while (true)
	{

		double newTime = now();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		while (frameTime > 0.0)
		{
			float deltaTime = std::min(frameTime, dt);
			update();

			frameTime -= deltaTime;
			t += deltaTime;
		}

		
		render();
	}
}

void Engine::update()
{
	// Read user input
	input.read();

	// Update systems

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
