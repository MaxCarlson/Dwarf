#include "stdafx.h"
#include "Engine.h"

// Components
#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\Components\ItemStored.h"
#include "ECS\Components\HealthComponent.h"
#include "ECS\Components\Item.h"
#include "ECS\Components\Claimed.h"
#include "ECS\Components\Building.h"
#include "ECS\Components\ItemCarried.h"
#include "ECS\Components\Stockpile.h"
#include "ECS\Components\Calender.h"

// Component Tags
#include "ECS\Components\Tags\MiningTag.h"
#include "ECS\Components\Tags\BuilderTag.h"
#include "ECS\Components\Tags\WorkOrderTag.h"
#include "ECS\Components\Tags\HaulingTag.h"
#include "ECS\Components\Tags\LumberjacTag.h"
#include "ECS\Components\Tags\HarvestTag.h"
#include "ECS\Components\Tags\PlantingTag.h"

// Component Sentients
#include "ECS\Components\Sentients\Stats.h"
#include "ECS\Components\Sentients\Inventory.h"
#include "ECS\Components\Sentients\AiWorkComponent.h"

// Systems
#include "ECS\Systems\RenderSystem.h"
#include "ECS\Systems\MovementSystem.h"
#include "ECS\Systems\ai\MovementAiSystem.h"
#include "ECS\Systems\MiningSystem.h"
#include "ECS\Systems\DijkstraSystems\DijkstraMapsHandler.h"
#include "ECS\Systems\EntityPositionCache.h"

// Gui systems
#include "ECS\Systems\Gui\InputHandler.h"
#include "ECS\Systems\Gui\GuiSystem.h"

// Ai systems
#include "ECS\Systems\ai\EquipHandler.h"
#include "ECS\Systems\ai\AiWorkSystem.h"
#include "ECS\Systems\ai\MiningAi.h"
#include "ECS\Systems\ai\BuildAi.h"
#include "ECS\Systems\ai\WorkOrders.h"
#include "ECS\Systems\ai\StockpileSystem.h"
#include "ECS\Systems\ai\HaulingSystem.h"
#include "ECS\Systems\ai\AiArchitecture.h"
#include "ECS\Systems\ai\WoodcuttingAi.h"
#include "ECS\Systems\ai\HarvestAi.h"
#include "ECS\Systems\ai\FarmingAi.h"

// System Helpers
#include "ECS\Systems\helpers\ItemHelper.h"
#include "ECS\Systems\helpers\DesignationHandler.h"
#include "ECS\Systems\helpers\WorkOrderHelper.h"

// System time based
#include "ECS\Systems\timebased\CalenderSystem.h"
#include "ECS\Systems\timebased\PlantSystem.h"

#include "Coordinates.h"
#include "Designations.h"
#include "Map/Map.h"
#include "Map/MapRender.h"
#include "Raws\DefInfo.h"
#include "Map\building\regionBuilder.h"
#include "Helpers\Rng.h"
#include "Map\building\PlanetBuilding.h"

// globals
#include "Globals\global_calender.h"

#include <chrono>
#include <cereal.hpp>
#include "../cereal/archives/binary.hpp"
#include "../cereal/types/memory.hpp"
#include "../cereal/types/vector.hpp"

typedef std::chrono::milliseconds::rep TimePoint;
inline TimePoint now() {
	return std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::steady_clock::now().time_since_epoch()).count();
}

Engine::~Engine()
{
}

void Engine::newGame(int px, int py, int screenWidth, int screenHeight)
{
	mapRenderer = std::make_unique<MapRender>();

	// Add in a seed selection process
	buildRegion(planet, px, py, { screenWidth, screenHeight, MAX_ZLVL }, rng);

	// Init misc maps and designations
	calender = std::make_unique<Calender>(); // Eventually give an input date based on world generation
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
	iarchive(planet);
	load_region(dirpath);
	world.load(iarchive);

	// Init misc maps and designations
	calender = std::make_unique<Calender>();
	mapRenderer = std::make_unique<MapRender>(); // ~~~~ Get rid of this and combine render systems
	defInfo = std::make_unique<DefInfo>();

	// Misc archives, move somewhere else
	iarchive(calender);
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

	// Save Planet then region then ECS world
	archive(planet);
	save_region(dirpath);
	world.save(archive);

	// Misc archives, move somewhere else
	archive(calender);
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
	world.registerComponent<AiWorkComponent>(); 
	world.registerComponent<Stats>();
	world.registerComponent<Inventory>();
	world.registerComponent<ItemCarried>();
	world.registerComponent<ItemStored>();
	world.registerComponent<Item>();
	world.registerComponent<HealthComponent>();
	world.registerComponent<MiningTag>();
	world.registerComponent<BuilderTag>();	
	world.registerComponent<Claimed>();
	world.registerComponent<Building>();
	world.registerComponent<Stockpile>();

	world.registerComponent<MiningTag>();
	world.registerComponent<BuilderTag>();
	world.registerComponent<WorkOrderTag>();
	world.registerComponent<HaulingTag>();
	world.registerComponent<ArchitectTag>();
	world.registerComponent<LumberjacTag>();
	world.registerComponent<HarvestTag>();
	world.registerComponent<PlantingTag>();

	// Global components
	world.registerComponent<Calender>();
}

void Engine::init()
{		
	// Ensure component Index's match before and after
	// serialization
	regComponents();

	//RunSystems::initSystems(); // Work on better format for the systems in the future. Also move them out of this file

	// Non archived extern items
	positionCache = std::make_unique<PositionCache>();
	workOrderHelper = std::make_unique<WorkOrderHelper>();

	// Init raws info, holds other misc 
	// game stats as well
	defInfo->init();

	// Add systems at init
	guiSystem = new GuiSystem();
	renderSystem = new RenderSystem();
	movementSystem = new MovementSystem();
	movementAiSystem = new MovementAiSystem();
	aiWorkSystem = new AiWorkSystem();
	miningAi = new MiningAi();
	buildAi = new BuildAi();
	workOrders = new WorkOrders();
	stockpileSystem = new StockpileSystem();
	haulingSystem = new HaulingSystem();
	aiArchitecture = new AiArchitecture();
	woodcuttingAi = new WoodcuttingAi();
	harvestAi = new HarvestAi();
	farmingAi = new FarmingAi();

	// Non Entity or non Updating Systems
	calenderSystem = new CalenderSystem();
	inputHandler = new InputHandler();
	designationHandler = new DesignationHandler();
	miningSystem = new MiningSystem();
	dijkstraHandler = new DijkstraMapsHandler();
	entityPositionCache = new EntityPositionCache();
	equipHandler = new EquipHandler();
	plantSystem = new PlantSystem();


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
	world.addSystem(*stockpileSystem);
	world.addSystem(*haulingSystem);
	world.addSystem(*aiArchitecture);
	world.addSystem(*woodcuttingAi);
	world.addSystem(*calenderSystem);
	world.addSystem(*plantSystem);
	world.addSystem(*harvestAi);
	world.addSystem(*farmingAi);

	// Init systems 
	miningSystem->init();
	dijkstraHandler->init();
	entityPositionCache->init();
	equipHandler->init();
	miningAi->init();
	designationHandler->init();
	buildAi->init();
	workOrders->init();
	stockpileSystem->init();
	haulingSystem->init();
	aiArchitecture->init();
	woodcuttingAi->init();
	harvestAi->init();
	
	world.refresh();

	run();
}

// Game loop
void Engine::run()
{
	bool majorTick = false;
	double majorTickCounter = 0.0;

	const double MS_PER_UPDATE = 17.0;
	double previous = double(now());
	double lag = 0.0;

	while (true)
	{
		double current = double(now());
		double elapsed = current - previous;
		previous = current;
		lag += elapsed;

		//input.read();
		inputHandler->update(lag);

		if (current_game_state != Engine::PAUSED) 
		{
			while (lag >= MS_PER_UPDATE) // Switch this to only update in steps and not compensate for lag? Probably should...
			{
				if (majorTickCounter > 250.0)
				{
					majorTick = true;
					majorTickCounter = 0.0;
				}

				update(MS_PER_UPDATE, majorTick);

				//lag -= MS_PER_UPDATE;
				majorTickCounter += MS_PER_UPDATE;
				lag = 0.0; // constant steps for the moment
				majorTick = false;
			}
		}

		else
			lag = 0.0;

		render(lag);

		if (current_game_state == Engine::TO_MAIN_MENU)
		{
			terminal_clear();
			return;
		}
			
	}
}

void Engine::update(double deltaTime, bool majorTick)
{
	// Deliver any deffered messages to systems
	world.deliver_messages();

	// Operations only done every major tick
	if (majorTick)
	{
		calenderSystem->update(deltaTime);
		workOrderHelper->update(deltaTime);
	}

	// Update systems
	miningSystem->update(deltaTime);
	movementSystem->update(deltaTime);
	dijkstraHandler->update(deltaTime);

	// Work Systems

	// Assign jobs to entities
	// TODO : Add more depth to job evaluators, relying on more
	// than just distance things such as skill level, other jobs, other entities, etc
	aiWorkSystem->update(deltaTime);

	// Perform assigned jobs
	workOrders->update(deltaTime);
	miningAi->update(deltaTime);
	woodcuttingAi->update(deltaTime);
	buildAi->update(deltaTime);
	aiArchitecture->update(deltaTime);
	harvestAi->update(deltaTime);

	stockpileSystem->update(deltaTime);
	haulingSystem->update(deltaTime);
	plantSystem->update(deltaTime);

	// Update all traits in entities that were killed, deactivated,
	// or activated (when a component changes)
	world.refresh();
}

void Engine::render(const double duration)
{	
	// Clear terminal before render
	terminal_clear();

	// Render Entities
	renderSystem->update(duration);

	// Render the gui elements last,
	// just in case something is showing through ~~ Make this a system
	// that shouldn't
	guiSystem->render();

	// Refresh terminal window to display
	// render changes
	terminal_refresh();
}
