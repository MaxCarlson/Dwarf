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
#include "ECS\Systems\Gui\DesignWoodcutting.h"
#include "ECS\Systems\Gui\DesignWorkOrders.h"
#include "ECS\Systems\Gui\JobPreferencesUi.h"
#include "ECS\Systems\Gui\DesignStockpiles.h"

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

// System Helpers
#include "ECS\Systems\helpers\ItemHelper.h"
#include "ECS\Systems\helpers\DesignationHandler.h"
#include "ECS\Systems\helpers\WorkOrderHelper.h"
#include "ECS\Systems\helpers\RegionHelper.h"
#include "ECS\Systems\helpers\SeedsHelper.h"

// System time based
#include "ECS\Systems\timebased\CalenderSystem.h"
#include "ECS\Systems\timebased\PlantSystem.h"

// Non systems
#include "ECS\Systems\EntityPositionCache.h"
#include "Globals\global_calender.h"
#include "Designations.h"
#include "Raws\DefInfo.h"
#include "mouse.h"
#include "KeyDampener.h"
#include <iomanip> 
#include <imgui.h>
#include <DwarfRender.h>

const std::string RENDER_SYSTEM = "Render System";
const std::string MOVEMENT_SYSTEM = "Movement System";
const std::string MOVEMENT_AI_SYSTEM = "Movement Ai System";
const std::string AI_WORK_SYSTEM = "Ai Work System";
const std::string MINING_AI = "Mining Ai";
const std::string BUILD_AI = "Build Ai";
const std::string WORK_ORDERS_SYSTEM = "Work Orders";
const std::string WORK_ORDER_HELPER = "Work Order Helper";
const std::string STOCKPILE_SYSTEM = "Stockpile System";
const std::string HAULING_SYSTEM = "Hauling System";
const std::string INPUT_HANDLER = "Input Handler";
const std::string DESIGNATION_HANDLER = "Designation Handler";
const std::string MINING_SYSTEM = "Mining System";
const std::string DIJKSTRA_MAPS_HANDLER = "Dijkstra Maps System";
const std::string ENTITY_POSITION_CACHE = "Position Cache";
const std::string EQUIP_HANDLER = "Equip Handler";
const std::string ITEM_HELPER = "Item Helper";
const std::string AI_ARCHITECTURE = "Ai Architecture";
const std::string WOODCUTTING_AI = "Woodcutting Ai";
const std::string HARVEST_AI = "Harvest Ai";
const std::string FARMING_AI = "Farming Ai";
const std::string CALENDER_SYSTEM = "Calender System";
const std::string PLANT_SYSTEM = "Plant System";
const std::string REGION_HELPER = "Region Helper";
const std::string FARM_CLEAR_AI = "Farm Clear Ai";
const std::string FARM_SOIL_AI = "Farm Soil Ai";
const std::string SEED_HELPER = "Seed Helper";

// Gui Systems
const std::string MENU_BAR = "Menu Bar";
const std::string DESIGN_ARCHITECTURE = "Design Architecture";
const std::string DESIGN_BUILDING = "Design Building";
const std::string DESIGN_DEV_MODE = "Design Dev Mode";
const std::string DESIGN_HARVEST = "Design Harvest";
const std::string DESIGN_MINING = "Design Mining";
const std::string DESIGN_WOODCUTTING = "Design Woodcutting";
const std::string DESIGN_WORKORDERS = "Design Work Orders";
const std::string DESIGN_STOCKPILES = "Design Stockpiles";
const std::string CAMERA_SYSTEM = "Camera System";
const std::string JOB_PREFERENCES_UI = "Job Preferences UI";


namespace RunSystems
{

boost::container::flat_map<std::string, SystemBase*> systems;
boost::container::flat_map<std::string, std::pair<int, std::vector<float>>> runTimes;


void initSystems(bool fromLoad)
{	
	// External system like Objects that aren't Systems
	positionCache = std::make_unique<PositionCache>();

	if (!fromLoad)
	{
		calender = std::make_unique<Calender>(); // Eventually give an input date based on world generation
		designations = std::make_unique<Designations>();
		defInfo = std::make_unique<DefInfo>();
	}

	// Grab all the info about defs we'd like
	// to have easy access to
	defInfo->init();

	// External systems
	workOrderHelper = std::make_unique<WorkOrderHelper>();
	regionHelper = std::make_unique<RegionHelper>();

	// Normal Systems
	// Memory is managed from inside the World.h class! Is it?
	systems[DESIGNATION_HANDLER] = new DesignationHandler;
	systems[RENDER_SYSTEM] = new RenderSystem;
	systems[MOVEMENT_SYSTEM] = new MovementSystem;
	systems[MOVEMENT_AI_SYSTEM] = new MovementAiSystem;
	systems[MINING_SYSTEM] = new MiningSystem;
	systems[AI_WORK_SYSTEM] = new AiWorkSystem;
	systems[MINING_AI] = new MiningAi;
	systems[DIJKSTRA_MAPS_HANDLER] = new DijkstraMapsHandler;
	systems[ENTITY_POSITION_CACHE] = new EntityPositionCache;
	systems[EQUIP_HANDLER] = new EquipHandler;
	systems[BUILD_AI] = new BuildAi;
	systems[WORK_ORDERS_SYSTEM] = new WorkOrders;
	systems[STOCKPILE_SYSTEM] = new StockpileSystem;
	systems[HAULING_SYSTEM] = new HaulingSystem;
	systems[AI_ARCHITECTURE] = new AiArchitecture;
	systems[WOODCUTTING_AI] = new WoodcuttingAi;
	systems[HARVEST_AI] = new HarvestAi;
	systems[FARMING_AI] = new FarmingAi;
	systems[CALENDER_SYSTEM] = new CalenderSystem;
	systems[PLANT_SYSTEM] = new PlantSystem;
	systems[FARM_CLEAR_AI] = new FarmingClearAi;
	systems[FARM_SOIL_AI] = new FarmingSoilAi;

	// Gui systems
	systems[MENU_BAR] = new MenuBar;
	systems[CAMERA_SYSTEM] = new CameraSystem;
	systems[DESIGN_ARCHITECTURE] = new DesignArchitecture;
	systems[DESIGN_BUILDING] = new DesignBuilding;
	systems[DESIGN_DEV_MODE] = new DesignDevMode;
	systems[DESIGN_HARVEST] = new DesignHarvest;
	systems[DESIGN_MINING] = new DesignMining;
	systems[DESIGN_WOODCUTTING] = new DesignWoodcutting;
	systems[DESIGN_WORKORDERS] = new DesignWorkOrders;
	systems[DESIGN_STOCKPILES] = new DesignStockpiles;
	systems[JOB_PREFERENCES_UI] = new JobPreferencesUi;

	// External Systems
	systems[ITEM_HELPER] = &itemHelper;
	systems[WORK_ORDER_HELPER] = workOrderHelper.get();
	systems[REGION_HELPER] = regionHelper.get();
	systems[SEED_HELPER] = &seedsHelper;

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

	// External Systems
	world.addSystem( itemHelper);
	world.addSystem(*workOrderHelper.get());
	world.addSystem(*regionHelper.get());
	world.addSystem( seedsHelper);

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

	constexpr double MS_PER_UPDATE = 17.0;
	constexpr double MS_PER_MAJOR_TICK = 250.0;

	static double tick = 0.0;
	static double majorTick = 0.0;

	tick += duration;
	majorTick += duration;


	runSystem(CAMERA_SYSTEM, duration);

	if (tick > MS_PER_UPDATE)
	{
		tick = 0.0;

		// Clear previous iterations of rendering 
		// from root consoles and layers manually here
		dfr::terminal->clear();
		if (dfr::gui)
			dfr::gui->clearAllLayers();

		// Operations only done every major tick
		if (majorTick > MS_PER_MAJOR_TICK)
		{
			majorTick = 0.0;
			runSystem(CALENDER_SYSTEM, MS_PER_MAJOR_TICK);
			runSystem(WORK_ORDER_HELPER, MS_PER_MAJOR_TICK);
		}

		// Update systems
		runSystem(MINING_SYSTEM, MS_PER_UPDATE);
		runSystem(MOVEMENT_SYSTEM, MS_PER_UPDATE);
		runSystem(DIJKSTRA_MAPS_HANDLER, MS_PER_UPDATE);

		runSystem(AI_WORK_SYSTEM, MS_PER_UPDATE);

		// Perform assigned jobs
		runSystem(WORK_ORDERS_SYSTEM, MS_PER_UPDATE);
		runSystem(MINING_AI, MS_PER_UPDATE);
		runSystem(WOODCUTTING_AI, MS_PER_UPDATE);
		runSystem(BUILD_AI, MS_PER_UPDATE);
		runSystem(AI_ARCHITECTURE, MS_PER_UPDATE);
		runSystem(HARVEST_AI, MS_PER_UPDATE);
		runSystem(FARM_CLEAR_AI, MS_PER_UPDATE);
		runSystem(FARM_SOIL_AI, MS_PER_UPDATE);
		runSystem(FARMING_AI, MS_PER_UPDATE);

		// Perfrom mining and later constructing jobs?
		runSystem(REGION_HELPER, MS_PER_UPDATE);

		runSystem(STOCKPILE_SYSTEM, MS_PER_UPDATE);
		runSystem(HAULING_SYSTEM, MS_PER_UPDATE);
		runSystem(PLANT_SYSTEM, MS_PER_UPDATE);

		// Main Rendering 
		runSystem(RENDER_SYSTEM, MS_PER_UPDATE);
	}

	// Menu and gui rendering
	runSystem(MENU_BAR, duration); 

	if (gameState == GameState::DESIGN)
	{
		if (designState == DesignStates::ARCHITECURE) 
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
	}

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


