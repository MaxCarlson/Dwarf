#include "Engine.h"
#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\Components\KeyBoardComponent.h"
#include "ECS\Components\ItemStored.h"
//#include "ECS\Components\CombatStatsComponent.h"
#include "ECS\Components\HealthComponent.h"
#include "ECS\Components\Item.h"
#include "ECS\Components\LaborStatsComponent.h"
#include "ECS\Components\Tags\BuilderTag.h"
#include "ECS\Systems\RenderSystem.h"
#include "ECS\Systems\MovementSystem.h"
#include "ECS\Systems\ai\MovementAiSystem.h"
#include "ECS\Systems\MiningSystem.h"
#include "ECS\Systems\DijkstraSystems\DijkstraMapsHandler.h"
#include "ECS\Systems\EntityPositionCache.h"
#include "ECS\Systems\ai\EquipHandler.h"
#include "ECS\Systems\ai\AiWorkSystem.h"
#include "ECS\Systems\ai\MiningAi.h"
#include "ECS\Systems\ai\BuildAi.h"
#include "Coordinates.h"
#include "Designations.h"
#include "Map/Map.h"
#include "Map/MapRender.h"

#include "BearLibTerminal.h"
#include <unordered_map>
#include <chrono>
#include <cereal.hpp>
#include "../cereal/archives/binary.hpp"
#include "../cereal/types/memory.hpp"
#include "../cereal/types/vector.hpp"
#include <fstream>



typedef std::chrono::milliseconds::rep TimePoint;
inline TimePoint now() {
	return std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::steady_clock::now().time_since_epoch()).count();
}

Engine::~Engine()
{
}

void Engine::newGame(int screenWidth, int screenHeight)
{
	mapRenderer = std::make_unique<MapRender>();

	// Create map
	map = std::make_unique<Map>(screenWidth, screenHeight, MAX_ZLVL);

	// Init misc maps and designations
	designations = std::make_unique<Designations>();
	positionCache = std::make_unique<std::unordered_multimap<int, std::size_t>>();

	// Init systems
	init();
}

void Engine::loadGame(std::string filePath)
{
	std::string dirpath = filePath;

	std::ifstream is(dirpath, std::ios::binary);
	cereal::BinaryInputArchive iarchive(is);

	//std::ifstream is(dirpath); // JSON
	//cereal::JSONInputArchive iarchive(is);

	// Load region then world
	// Systems aren't loaded, just re-created
	load_region(dirpath);
	world.load(iarchive);

	// Misc archives, move somewhere else
	iarchive(designations);
	//iarchive(positionCache);

	// Init misc maps and designations
	positionCache = std::make_unique<std::unordered_multimap<int, std::size_t>>();
	mapRenderer = std::make_unique<MapRender>();

	init();
}

void Engine::saveGame(std::string filePath)
{
	std::string dirpath = "Saves/" + filePath;

	std::ofstream os(dirpath, std::ios::binary);
	cereal::BinaryOutputArchive archive(os);

	//std::ofstream os(dirpath);  // JSON
	//cereal::JSONOutputArchive archive(os);

	// Save region then world
	save_region(dirpath);
	world.save(archive);

	// Misc archives, move somewhere else
	archive(designations);
//	archive(positionCache);

}

// This is a placeholder untill ECS no longer requires components
// to be in the same order
void Engine::regComponents()
{ 
	world.registerComponent<PositionComponent>();
	world.registerComponent<MovementComponent>();
	world.registerComponent<RenderComponent>();
	world.registerComponent<LaborStatsComponent>();
	world.registerComponent<JobComponent>();
	world.registerComponent<Inventory>();
	world.registerComponent<ItemStored>();
	world.registerComponent<Item>();
	world.registerComponent<HealthComponent>();
	//world.registerComponent<CombatStatsComponent>();
	world.registerComponent<MiningTag>();
	world.registerComponent<BuilderTag>();	
}

void Engine::init()
{		
	// Ensure component Index's match before and after
	// serialization
	regComponents();

	// Add systems at init
	renderSystem = new RenderSystem();
	movementSystem = new MovementSystem();
	movementAiSystem = new MovementAiSystem();
	aiWorkSystem = new AiWorkSystem();
	miningAi = new MiningAi();
	miningAi->init();
	buildAi = new BuildAi();
	buildAi->init();

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
	world.addSystem(*buildAi);

	// Init systems 
	miningSystem->init();
	dijkstraHandler->init();
	entityPositionCache->init();
	equipHandler->init();
	
	world.refresh();

	run();
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

		if (current_game_state != Engine::PAUSED) 
		{
			while (lag >= MS_PER_UPDATE) // Switch this to only update in steps and not compensate for lag? Probably should...
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
	buildAi->update(deltaTime);

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

	// Render the local map !~!~~~ Replace this swith system?
	mapRenderer->render();

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
