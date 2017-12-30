#include "Engine.h"

// Components
#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\Components\KeyBoardComponent.h"
#include "ECS\Components\ItemStored.h"
#include "ECS\Components\HealthComponent.h"
#include "ECS\Components\Item.h"
#include "ECS\Components\LaborStatsComponent.h"
#include "ECS\Components\Tags\BuilderTag.h"
#include "ECS\Components\Claimed.h"
#include "ECS\Components\Building.h"

// Component Tags
#include "ECS\Components\Tags\MiningTag.h"
#include "ECS\Components\Tags\BuilderTag.h"
#include "ECS\Components\Tags\WorkOrderTag.h"

// Systems
#include "ECS\Systems\RenderSystem.h"
#include "ECS\Systems\MovementSystem.h"
#include "ECS\Systems\ai\MovementAiSystem.h"
#include "ECS\Systems\MiningSystem.h"
#include "ECS\Systems\DijkstraSystems\DijkstraMapsHandler.h"
#include "ECS\Systems\EntityPositionCache.h"
#include "ECS\Systems\InputHandler.h"

// Ai systems
#include "ECS\Systems\ai\EquipHandler.h"
#include "ECS\Systems\ai\AiWorkSystem.h"
#include "ECS\Systems\ai\MiningAi.h"
#include "ECS\Systems\ai\BuildAi.h"
#include "ECS\Systems\ai\WorkOrders.h"

// System Helpers
#include "ECS\Systems\helpers\ItemHelper.h"
#include "ECS\Systems\helpers\DesignationHandler.h"
#include "ECS\Systems\helpers\WorkOrderHelper.h"

#include "Coordinates.h"
#include "Designations.h"
#include "Map/Map.h"
#include "Map/MapRender.h"
#include "Raws\DefInfo.h"

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
	map = std::make_unique<Map>(screenWidth, screenHeight, MAX_ZLVL); // Delete this and move creat code here

	// Init misc maps and designations
	designations = std::make_unique<Designations>();
	defInfo = std::make_unique<DefInfo>();

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

	// Init misc maps and designations
	mapRenderer = std::make_unique<MapRender>(); // ~~~~ Get rid of this and combine render systems
	defInfo = std::make_unique<DefInfo>();

	// Misc archives, move somewhere else
	iarchive(designations);
	iarchive(defInfo);

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
	archive(defInfo);

}

// This is a placeholder untill ECS no longer requires components
// to be in the same order
void Engine::regComponents()
{ 
	world.registerComponent<PositionComponent>();
	world.registerComponent<MovementComponent>();
	world.registerComponent<RenderComponent>();
	world.registerComponent<LaborStatsComponent>();
	world.registerComponent<JobComponent>(); // Delte this component perma
	world.registerComponent<Inventory>();
	world.registerComponent<ItemStored>();
	world.registerComponent<Item>();
	world.registerComponent<HealthComponent>();
	world.registerComponent<MiningTag>();
	world.registerComponent<BuilderTag>();	
	world.registerComponent<Claimed>();
	world.registerComponent<Building>();

	world.registerComponent<MiningTag>();
	world.registerComponent<BuilderTag>();
	world.registerComponent<WorkOrderTag>();
}

void Engine::init()
{		
	// Ensure component Index's match before and after
	// serialization
	regComponents();

	// Non archived extern items
	positionCache = std::make_unique<PositionCache>();
	workOrderHelper = std::make_unique<WorkOrderHelper>();

	// Init raws info, holds other misc 
	// game stats as well
	defInfo->init();

	// Add systems at init
	renderSystem = new RenderSystem();
	movementSystem = new MovementSystem();
	movementAiSystem = new MovementAiSystem();
	aiWorkSystem = new AiWorkSystem();
	miningAi = new MiningAi();
	buildAi = new BuildAi();
	workOrders = new WorkOrders();

	// Non Entity or non Updating Systems
	inputHandler = new InputHandler();
	designationHandler = new DesignationHandler();
	miningSystem = new MiningSystem();
	dijkstraHandler = new DijkstraMapsHandler();
	entityPositionCache = new EntityPositionCache();
	equipHandler = new EquipHandler();


	// Introduce systems to the world ~~ These Systems no longer need to be pointers?
	world.addSystem(*inputHandler);
	world.addSystem(*designationHandler);
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
	world.addSystem(itemHelper);
	world.addSystem(*workOrders);
	world.addSystem(*workOrderHelper);

	// Init systems 
	miningSystem->init();
	dijkstraHandler->init();
	entityPositionCache->init();
	equipHandler->init();
	miningAi->init();
	designationHandler->init();
	buildAi->init();
	workOrders->init();


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

		//input.read();
		inputHandler->update();

		if (current_game_state != Engine::PAUSED) 
		{
			while (lag >= MS_PER_UPDATE) // Switch this to only update in steps and not compensate for lag? Probably should...
			{
				update(MS_PER_UPDATE);
				//lag -= MS_PER_UPDATE;
				lag = 0.0; // constant steps for the moment
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
	workOrders->update(deltaTime);
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

	// Render Entities
	renderSystem->update();

	// Render the gui elements last,
	// just in case something is showing through ~~ Make this a system
	// that shouldn't
	gui.render();

	// Refresh terminal window to display
	// render changes
	terminal_refresh();
}
