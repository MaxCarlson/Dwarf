#include "stdafx.h"
#include "RunSystems.h"
#include "ECS\Systems.h"
#include "Globals\GlobalWorld.h"
#include "Globals\game_states.h"

#include "ECS\Systems\RenderSystem.h"
#include "ECS\Systems\MovementSystem.h"
#include "ECS\Systems\ai\MovementAiSystem.h"
#include "ECS\Systems\MiningSystem.h"
#include "ECS\Systems\DijkstraSystems\DijkstraMapsHandler.h"

// Gui systems
#include "ECS\Systems\Gui\MenuBar.h"
#include "ECS\Systems\Gui\CameraSystem.h"
#include "ECS\Systems\Gui\DesignArchitecture.h"
#include "ECS\Systems\Gui\DesignBuilding.h"
#include "ECS\Systems\Gui\DesignDevMode.h"
#include "ECS\Systems\Gui\DesignHarvest.h"
#include "ECS\Systems\Gui\DesignMining.h"
#include "ECS\Systems\Gui\DesignWoodcutting.h" // TODO: Convert all these gui "Systems" To non classes that are not systems if they do not filter entities
#include "ECS\Systems\Gui\DesignWorkOrders.h"
#include "ECS\Systems\Gui\JobPreferencesUi.h"
#include "ECS\Systems\Gui\DesignStockpiles.h"
#include "ECS\Systems\Gui\DwarfInfoGui.h"
#include "ECS\Systems\Gui\ClickSystemGui.h"
#include "ECS\Systems\Gui\MilitaryGui.h"

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
#include "ECS\Systems\ai\FarmingClearAi.h"
#include "ECS\Systems\ai\FarmingSoilAi.h"
#include "ECS\Systems\ai\ViewSystem.h"

// Needs systems
#include "ECS\Systems\ai\Needs\NeedsSystem.h"
#include "ECS\Systems\ai\Needs\SleepSystem.h"
#include "ECS\Systems\ai\Needs\EatFoodSystem.h"
#include "ECS\Systems\ai\Needs\JoySystem.h"

// System Helpers
#include "ECS\Systems\helpers\ItemHelper.h"
#include "ECS\Systems\helpers\DesignationHandler.h"
#include "ECS\Systems\helpers\WorkOrderHelper.h"
#include "ECS\Systems\helpers\RegionHelper.h"
#include "ECS\Systems\helpers\SeedsHelper.h"
#include "ECS\Systems\helpers\SpawnCreature.h"

// System time based
#include "ECS\Systems\timebased\CalenderSystem.h"
#include "ECS\Systems\timebased\PlantSystem.h"

// Combat systems
#include "ECS\Systems\ai\Combat\DeathSystem.h"
#include "ECS\Systems\ai\Combat\ProcessAttacks.h"
#include "ECS\Systems\ai\Combat\DraftedSystem.h"

// Non systems
#include "ECS\Systems\EntityPositionCache.h"
#include "Globals\global_calender.h"
#include "Designations\Designations.h"
#include "Raws\DefInfo.h"
#include "mouse.h"
#include "KeyDampener.h"
#include <iomanip> 
#include <imgui.h>
#include <DwarfRender.h>

const std::string RENDER_SYSTEM			= "Render System";
const std::string MOVEMENT_SYSTEM		= "Movement System";
const std::string MOVEMENT_AI_SYSTEM	= "Movement Ai System";
const std::string AI_WORK_SYSTEM		= "Ai Work System";
const std::string MINING_AI				= "Mining Ai";
const std::string BUILD_AI				= "Build Ai";
const std::string WORK_ORDERS_SYSTEM	= "Work Orders";
const std::string STOCKPILE_SYSTEM		= "Stockpile System";
const std::string HAULING_SYSTEM		= "Hauling System";
const std::string INPUT_HANDLER			= "Input Handler";
const std::string MINING_SYSTEM			= "Mining System";
const std::string DIJKSTRA_MAPS_HANDLER = "Dijkstra Maps System";
const std::string ENTITY_POSITION_CACHE = "Position Cache";
const std::string EQUIP_HANDLER			= "Equip Handler";
const std::string AI_ARCHITECTURE		= "Ai Architecture";
const std::string WOODCUTTING_AI		= "Woodcutting Ai";
const std::string HARVEST_AI			= "Harvest Ai";
const std::string FARMING_AI			= "Farming Ai";
const std::string CALENDER_SYSTEM		= "Calender System";
const std::string PLANT_SYSTEM			= "Plant System";
const std::string FARM_CLEAR_AI			= "Farm Clear Ai";
const std::string FARM_SOIL_AI			= "Farm Soil Ai";
const std::string VIEW_SYSTEM			= "View System";

// Helper Systems
const std::string WORK_ORDER_HELPER		= "Work Order Helper";
const std::string DESIGNATION_HANDLER	= "Designation Handler";
const std::string ITEM_HELPER			= "Item Helper";
const std::string REGION_HELPER			= "Region Helper";
const std::string SEED_HELPER			= "Seed Helper";
const std::string SPAWN_CREATURE		= "Spawn Creature";

// Needs and passive
const std::string NEEDS_SYSTEM = "Needs System";
const std::string SLEEP_SYSTEM = "Sleep System";
const std::string EAT_FOOD_SYSTEM = "Eat Food System";
const std::string JOY_SYSTEM = "Joy System";

// Gui Systems
const std::string MENU_BAR = "Menu Bar";
const std::string DESIGN_ARCHITECTURE = "Design Architecture"; // TODO: Convert all these gui "Systems" To non classes that are not systems if they do not filter entities
const std::string DESIGN_BUILDING = "Design Building";
const std::string DESIGN_DEV_MODE = "Design Dev Mode";
const std::string DESIGN_HARVEST = "Design Harvest";
const std::string DESIGN_MINING = "Design Mining";
const std::string DESIGN_WOODCUTTING = "Design Woodcutting";
const std::string DESIGN_WORKORDERS = "Design Work Orders";
const std::string DESIGN_STOCKPILES = "Design Stockpiles";
const std::string CAMERA_SYSTEM = "Camera System";
const std::string JOB_PREFERENCES_UI = "Job Preferences UI";
const std::string DWARF_INFO_GUI = "Dwarf Info Gui";


// Combat
const std::string DEATH_SYSTEM = "Death System";
const std::string PROCESS_ATTACKS = "Process Attacks";
const std::string DRAFTED_SYSTEM = "Drafted System";

namespace RunSystems
{

boost::container::flat_map<std::string, SystemBase*> systems;
boost::container::flat_map<std::string, std::pair<int, std::vector<float>>> runTimes;


void cleanSystems()
{
	// Vardiadic systems are cleared in RegComponents as we need the componets to clear them

	// Delete all systems and all entities
	world.clear();
}

void initSystems(bool fromLoad)
{	
	// External system like Objects that aren't Systems
	positionCache = std::make_unique<PositionCache>();

	if (!fromLoad)
	{
		calender		= std::make_unique<Calender>(); // Eventually give an input date based on world generation
		designations	= std::make_unique<Designations>();
		defInfo			= std::make_unique<DefInfo>();
	}

	// Grab all the info about defs we'd like
	// to have easy access to
	defInfo->init();

	// External systems
	workOrderHelper = std::make_unique<WorkOrderHelper>();
	regionHelper	= std::make_unique<RegionHelper>();

	// Normal Systems
	// Memory is managed from inside the World.h class! Is it?
	systems[DESIGNATION_HANDLER]	= new DesignationHandler;
	systems[RENDER_SYSTEM]			= new RenderSystem;
	systems[MOVEMENT_SYSTEM]		= new MovementSystem;
	systems[MOVEMENT_AI_SYSTEM]		= new MovementAiSystem;
	systems[MINING_SYSTEM]			= new MiningSystem;
	systems[AI_WORK_SYSTEM]			= new AiWorkSystem;
	systems[MINING_AI]				= new MiningAi;
	systems[DIJKSTRA_MAPS_HANDLER]	= new DijkstraMapsHandler;
	systems[ENTITY_POSITION_CACHE]	= new EntityPositionCache;
	systems[EQUIP_HANDLER]			= new EquipHandler;
	systems[BUILD_AI]				= new BuildAi;
	systems[WORK_ORDERS_SYSTEM]		= new WorkOrders;
	systems[STOCKPILE_SYSTEM]		= new StockpileSystem;
	systems[HAULING_SYSTEM]			= new HaulingSystem;
	systems[AI_ARCHITECTURE]		= new AiArchitecture;
	systems[WOODCUTTING_AI]			= new WoodcuttingAi;
	systems[HARVEST_AI]				= new HarvestAi;
	systems[FARMING_AI]				= new FarmingAi;
	systems[CALENDER_SYSTEM]		= new CalenderSystem;
	systems[PLANT_SYSTEM]			= new PlantSystem;
	systems[FARM_CLEAR_AI]			= new FarmingClearAi;
	systems[FARM_SOIL_AI]			= new FarmingSoilAi;
	systems[VIEW_SYSTEM]			= new ViewSystem;

	// Needs 
	systems[NEEDS_SYSTEM]			= new NeedsSystem;
	systems[SLEEP_SYSTEM]			= new SleepSystem;
	systems[EAT_FOOD_SYSTEM]		= new EatFoodSystem;
	systems[JOY_SYSTEM]				= new JoySystem;

	// Gui systems
	systems[MENU_BAR]				= new MenuBar;
	systems[CAMERA_SYSTEM]			= new CameraSystem;
	systems[DESIGN_ARCHITECTURE]	= new DesignArchitecture;
	systems[DESIGN_BUILDING]		= new DesignBuilding;
	systems[DESIGN_DEV_MODE]		= new DesignDevMode;
	systems[DESIGN_HARVEST]			= new DesignHarvest;
	systems[DESIGN_MINING]			= new DesignMining;
	systems[DESIGN_WOODCUTTING]		= new DesignWoodcutting;
	systems[DESIGN_WORKORDERS]		= new DesignWorkOrders;
	systems[DESIGN_STOCKPILES]		= new DesignStockpiles;
	systems[JOB_PREFERENCES_UI]		= new JobPreferencesUi;
	systems[DWARF_INFO_GUI]			= new DwarfInfoGui;

	// Combat
	systems[DEATH_SYSTEM]			= new DeathSystem;
	systems[PROCESS_ATTACKS]		= new ProcessAttacks;
	systems[DRAFTED_SYSTEM]			= new DraftedSystem;

	// External/Helper Systems
	systems[ITEM_HELPER]			= &itemHelper;
	systems[WORK_ORDER_HELPER]		= workOrderHelper.get();
	systems[REGION_HELPER]			= regionHelper.get();
	systems[SEED_HELPER]			= &seedsHelper;
	systems[SPAWN_CREATURE]			= new SpawnCreature;

	// Add systems to world. Cast to their derived class so world 
	// doesn't interpret them as SystemBase's
	world.addSystem(* static_cast<DesignationHandler *>(systems[DESIGNATION_HANDLER]));
	world.addSystem(* static_cast<RenderSystem *>(systems[RENDER_SYSTEM]));
	world.addSystem(* static_cast<MovementSystem *>(systems[MOVEMENT_SYSTEM]));
	world.addSystem(* static_cast<MovementAiSystem *>(systems[MOVEMENT_AI_SYSTEM]));
	world.addSystem(* static_cast<MiningSystem *>(systems[MINING_SYSTEM]));
	world.addSystem(* static_cast<AiWorkSystem *>(systems[AI_WORK_SYSTEM]));
	world.addSystem(* static_cast<MiningAi *>(systems[MINING_AI]));
	world.addSystem(* static_cast<DijkstraMapsHandler *>(systems[DIJKSTRA_MAPS_HANDLER]));
	world.addSystem(* static_cast<EntityPositionCache *>(systems[ENTITY_POSITION_CACHE]));
	world.addSystem(* static_cast<EquipHandler *>(systems[EQUIP_HANDLER]));
	world.addSystem(* static_cast<BuildAi *>(systems[BUILD_AI]));
	world.addSystem(* static_cast<WorkOrders *>(systems[WORK_ORDERS_SYSTEM]));
	world.addSystem(* static_cast<StockpileSystem *>(systems[STOCKPILE_SYSTEM]));
	world.addSystem(* static_cast<HaulingSystem *>(systems[HAULING_SYSTEM]));
	world.addSystem(* static_cast<AiArchitecture *>(systems[AI_ARCHITECTURE]));
	world.addSystem(* static_cast<WoodcuttingAi *>(systems[WOODCUTTING_AI]));
	world.addSystem(* static_cast<HarvestAi *>(systems[HARVEST_AI]));
	world.addSystem(* static_cast<FarmingAi *>(systems[FARMING_AI]));
	world.addSystem(* static_cast<CalenderSystem *>(systems[CALENDER_SYSTEM]));
	world.addSystem(* static_cast<PlantSystem *>(systems[PLANT_SYSTEM]));
	world.addSystem(* static_cast<FarmingClearAi *>(systems[FARM_CLEAR_AI]));
	world.addSystem(* static_cast<FarmingSoilAi *>(systems[FARM_SOIL_AI]));
	world.addSystem(* static_cast<ViewSystem *>(systems[VIEW_SYSTEM]));


	// Needs 
	world.addSystem(* static_cast<NeedsSystem *>(systems[NEEDS_SYSTEM]));
	world.addSystem(* static_cast<SleepSystem *>(systems[SLEEP_SYSTEM]));
	world.addSystem(* static_cast<EatFoodSystem *>(systems[EAT_FOOD_SYSTEM]));
	world.addSystem(* static_cast<JoySystem *>(systems[JOY_SYSTEM]));

	// Gui Systems
	world.addSystem(* static_cast<MenuBar *>(systems[MENU_BAR]));
	world.addSystem(* static_cast<CameraSystem *>(systems[CAMERA_SYSTEM]));
	world.addSystem(* static_cast<DesignArchitecture *>(systems[DESIGN_ARCHITECTURE]));
	world.addSystem(* static_cast<DesignBuilding *>(systems[DESIGN_BUILDING]));
	world.addSystem(* static_cast<DesignDevMode *>(systems[DESIGN_DEV_MODE]));
	world.addSystem(* static_cast<DesignHarvest *>(systems[DESIGN_HARVEST]));
	world.addSystem(* static_cast<DesignMining *>(systems[DESIGN_MINING]));
	world.addSystem(* static_cast<DesignWoodcutting *>(systems[DESIGN_WOODCUTTING]));
	world.addSystem(* static_cast<DesignWorkOrders *>(systems[DESIGN_WORKORDERS]));
	world.addSystem(* static_cast<DesignStockpiles *>(systems[DESIGN_STOCKPILES]));
	world.addSystem(* static_cast<JobPreferencesUi *>(systems[JOB_PREFERENCES_UI]));
	world.addSystem(* static_cast<DwarfInfoGui *>(systems[DWARF_INFO_GUI]));

	// Combat
	world.addSystem(* static_cast<DeathSystem *>(systems[DEATH_SYSTEM]));
	world.addSystem(* static_cast<ProcessAttacks *>(systems[PROCESS_ATTACKS]));
	world.addSystem(* static_cast<DraftedSystem *>(systems[DRAFTED_SYSTEM]));

	// External Systems
	world.addSystem( itemHelper);
	world.addSystem(*workOrderHelper.get());
	world.addSystem(*regionHelper.get());
	world.addSystem( seedsHelper); // TODO:: This can be converted into a variadic system
	world.addSystem(* static_cast<SpawnCreature *>(systems[SPAWN_CREATURE]));


	for (auto& sys : systems)
		sys.second->init();

	world.refresh();
}

void runSystem(const std::string &name, const double duration)
{
	auto stime = std::chrono::high_resolution_clock::now();

	systems[name]->update(duration);

	auto etime = std::chrono::high_resolution_clock::now();
	auto rtime = std::chrono::duration_cast<std::chrono::microseconds>(etime - stime).count();

	auto find = runTimes.find(name);
	if (find != runTimes.end())
	{
		find->second.second[find->second.first] = static_cast<float>(rtime);
		++find->second.first;

		if (find->second.first >= 100)
			find->second.first = 0;
	}
	else
	{
		std::vector<float> times(100, 0.0);
		times[0] = static_cast<float>(rtime);
		runTimes[name] = std::make_pair(1, times);
	}
}

void updateSystems(const double duration)
{
	// Deliver any deffered messages to systems
	world.deliver_messages();

	keys::addTime(duration);
	mouse::readMouse();

	constexpr double MS_PER_UPDATE		= 17.0;
	constexpr double MS_PER_MAJOR_TICK	= 250.0;

	static double tick		= 0.0;
	static double majorTick = 0.0;

	tick		+= duration;
	majorTick	+= duration;


	runSystem(CAMERA_SYSTEM, duration);

	if (tick > MS_PER_UPDATE)
	{
		tick = 0.0;

		double updateTime = ( gameState == GameState::PAUSED ) ? 0.0 : MS_PER_UPDATE;

		// Clear previous iterations of rendering 
		// from root consoles and layers manually here
		dfr::terminal->clear();
		if (dfr::gui)
			dfr::gui->clearAllLayers();

		// Operations only done every major tick
		if (majorTick > MS_PER_MAJOR_TICK)
		{
			majorTick = 0.0;
			runSystem(WORK_ORDER_HELPER, MS_PER_MAJOR_TICK);
		}
		runSystem(VIEW_SYSTEM, updateTime); // TODO: Where shouuld this be placed? Leaning toward near end so comp changes are in effect asap?

		runSystem(NEEDS_SYSTEM, updateTime); // These two systems should probably be set to run in major ticks
		runSystem(CALENDER_SYSTEM, updateTime);

		// Systems of higher priority than work
		runSystem(DRAFTED_SYSTEM, updateTime);

		// Assign jobs to entities
		runSystem(AI_WORK_SYSTEM, updateTime);

		// These sytems just update things
		runSystem(MINING_SYSTEM, updateTime);
		runSystem(MOVEMENT_SYSTEM, updateTime);
		runSystem(DIJKSTRA_MAPS_HANDLER, updateTime);
		runSystem(STOCKPILE_SYSTEM, updateTime);

		// Perform assigned jobs
		runSystem(WORK_ORDERS_SYSTEM, updateTime);
		runSystem(MINING_AI, updateTime);
		runSystem(WOODCUTTING_AI, updateTime);
		runSystem(BUILD_AI, updateTime);
		runSystem(AI_ARCHITECTURE, updateTime);
		runSystem(HARVEST_AI, updateTime);
		runSystem(FARM_CLEAR_AI, updateTime);
		runSystem(FARM_SOIL_AI, updateTime);
		runSystem(FARMING_AI, updateTime);
		runSystem(HAULING_SYSTEM, updateTime);
		runSystem(PLANT_SYSTEM, updateTime);

		// Need systems are also run as varying priority jobs
		// based on how low the particular need is priority increases
		runSystem(SLEEP_SYSTEM, updateTime);
		runSystem(EAT_FOOD_SYSTEM, updateTime);
		runSystem(JOY_SYSTEM, updateTime);

		// Perfrom mining and later constructing jobs?
		runSystem(REGION_HELPER, updateTime);

		// Pickup and drop all items requested
		runSystem(EQUIP_HANDLER, updateTime);

		// Belated combat systems
		runSystem(PROCESS_ATTACKS, updateTime);
		runSystem(DEATH_SYSTEM, updateTime);

		// Main Rendering 
		runSystem(RENDER_SYSTEM, MS_PER_UPDATE);
	}

	// Menu and gui rendering
	runSystem(MENU_BAR, duration); 
	ClickSystem::update(duration);

	if (gameState == GameState::DESIGN)
	{
		if (designState == DesignStates::ARCHITECURE) // TODO: Convert all these gui "Systems" To non classes that are not systems if they do not filter entities
			runSystem(DESIGN_ARCHITECTURE, duration);

		if (designState == DesignStates::BUILD)
			runSystem(DESIGN_BUILDING, duration);

		if (designState == DesignStates::CHOP_TREES)
			runSystem(DESIGN_WOODCUTTING, duration);

		if (designState == DesignStates::DEV_MODE)
			runSystem(DESIGN_DEV_MODE, duration);

		if (designState == DesignStates::HARVEST)
			runSystem(DESIGN_HARVEST, duration);

		if (designState == DesignStates::MINING)
			runSystem(DESIGN_MINING, duration);

		if (designState == DesignStates::WORK_ORDERS)
			runSystem(DESIGN_WORKORDERS, duration);

		if (designState == DesignStates::STOCKPILES)
			runSystem(DESIGN_STOCKPILES, duration);

		if (designState == DesignStates::JOB_PREFERENCES)
			runSystem(JOB_PREFERENCES_UI, duration);

		if (designState == DesignStates::MILITARY_GUI)
			MilitaryGui::update(duration);
	}

	// Gui systems that do not auto pause
	if (designState == DesignStates::DWARF_INFO)
		runSystem(DWARF_INFO_GUI, duration);

	static bool profiler = false;

	if (keys::isKeyDown(sf::Keyboard::Key::P))
		profiler = !profiler;

	if (profiler)
	{
		ImGui::Begin("Profiler");

		// Get average frame times
		static int tidx = -1;
		constexpr size_t framesToAvg = 100;
		static std::vector<double> ftimes(framesToAvg, 0.0);

		if (tidx < framesToAvg - 1)
			++tidx;
		else
			tidx = 0;

		ftimes[tidx] = duration;

		double ftotal = 0.0;
		for (const auto& ts : ftimes)
			ftotal += ts;

		ftotal /= static_cast<double>(framesToAvg);

		ImGui::Text("Frame Time: %f", duration); 
		ImGui::Text("Avg Frame Time (100): %f", ftotal);

		const double frameFrac = 100.0 / duration;
		for (auto sys = runTimes.begin(); sys != runTimes.end(); ++sys)
		{
			ImGui::PlotLines(sys->first.c_str(), (const float *)&sys->second.second.at(0), 100); // Make this into a % of total frame time instead of just a time double
			
			// Get frame time % from the micro-second 
			// counter at current place in graph
			ImGui::SameLine();
			std::stringstream ss;
			const auto framePerc = (static_cast<double>(sys->second.second[sys->second.first]) / 1000.0) * frameFrac;
			ss << std::fixed << std::setprecision(2) << framePerc;
			ImGui::Text(ss.str().c_str());
		}

		ImGui::End();
	}

	// Update all traits in entities that were killed, deactivated,
	// or activated (when a component changes)
	world.refresh();
}


} // End namespace


