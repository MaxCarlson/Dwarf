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
#include "ECS\Systems\Gui\InputHandler.h"
#include "ECS\Systems\Gui\GuiSystem.h"

#include "ECS\Systems\Gui\MenuBar.h"
#include "ECS\Systems\Gui\CameraSystem.h"
#include "ECS\Systems\Gui\DesignArchitecture.h"
#include "ECS\Systems\Gui\DesignBuilding.h"
#include "ECS\Systems\Gui\DesignDevMode.h"
#include "ECS\Systems\Gui\DesignHarvest.h"
#include "ECS\Systems\Gui\DesignMining.h"
#include "ECS\Systems\Gui\DesignWoodcutting.h"
#include "ECS\Systems\Gui\DesignWorkOrders.h"

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

// Non systems
#include "ECS\Systems\EntityPositionCache.h"
#include "Globals\global_calender.h"
#include "Designations.h"
#include "Raws\DefInfo.h"
#include "mouse.h"
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

// Gui Systems
const std::string MENU_BAR = "Menu Bar";
const std::string DESIGN_ARCHITECTURE = "Design Architecture";
const std::string DESIGN_BUILDING = "Design Building";
const std::string DESIGN_DEV_MODE = "Design Dev Mode";
const std::string DESIGN_HARVEST = "Design Harvest";
const std::string DESIGN_MINING = "Design Mining";
const std::string DESIGN_WOODCUTTING = "Design Woodcutting";
const std::string DESIGN_WORKORDERS = "Design Work Orders";
const std::string CAMERA_SYSTEM = "Camera System";


namespace RunSystems
{

std::unordered_map<std::string, SystemBase*> systems;


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

	defInfo->init();

	// External systems
	workOrderHelper = std::make_unique<WorkOrderHelper>();

	// Normal Systems
	systems[INPUT_HANDLER] = new InputHandler;
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
	systems[ITEM_HELPER] = new ItemHelper;
	systems[WORK_ORDERS_SYSTEM] = new WorkOrders;
	systems[WORK_ORDER_HELPER] = workOrderHelper.get();
	systems[STOCKPILE_SYSTEM] = new StockpileSystem;
	systems[HAULING_SYSTEM] = new HaulingSystem;
	systems[AI_ARCHITECTURE] = new AiArchitecture;
	systems[WOODCUTTING_AI] = new WoodcuttingAi;
	systems[HARVEST_AI] = new HarvestAi;
	systems[FARMING_AI] = new FarmingAi;
	systems[CALENDER_SYSTEM] = new CalenderSystem;
	systems[PLANT_SYSTEM] = new PlantSystem;

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

	// Add systems to world. Cast to their derived class so world 
	// doesn't interpret them as SystemBase's
	world.addSystem(* static_cast<InputHandler *>(systems[INPUT_HANDLER]));
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
	world.addSystem(* static_cast<ItemHelper *>(systems[ITEM_HELPER]));
	world.addSystem(* static_cast<WorkOrders *>(systems[WORK_ORDERS_SYSTEM]));
	world.addSystem(* static_cast<WorkOrderHelper *>(systems[WORK_ORDER_HELPER]));
	world.addSystem(* static_cast<StockpileSystem *>(systems[STOCKPILE_SYSTEM]));
	world.addSystem(* static_cast<HaulingSystem *>(systems[HAULING_SYSTEM]));
	world.addSystem(* static_cast<AiArchitecture *>(systems[AI_ARCHITECTURE]));
	world.addSystem(* static_cast<WoodcuttingAi *>(systems[WOODCUTTING_AI]));
	world.addSystem(* static_cast<HarvestAi *>(systems[HARVEST_AI]));
	world.addSystem(* static_cast<FarmingAi *>(systems[FARMING_AI]));
	world.addSystem(* static_cast<CalenderSystem *>(systems[CALENDER_SYSTEM]));
	world.addSystem(* static_cast<PlantSystem *>(systems[PLANT_SYSTEM]));

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

	for (auto& sys : systems)
		sys.second->init();

	world.refresh();
}

void runSystem(const std::string &name, const double duration)
{
	systems[name]->update(duration);
}

void updateSystems(const double duration)
{
	// Deliver any deffered messages to systems
	world.deliver_messages();

	// Read mouse in
	mouse::readMouse();

	// Clear previous iterations of rendering 
	// from root consoles and layers manually here
	dfr::terminal->clear();
	if (dfr::gui)
		dfr::gui->clearAllLayers();

	static double majorTick = 0.0;

	majorTick += duration;

	// Operations only done every major tick
	if (majorTick > 250.0)
	{
		majorTick = 0.0;
		runSystem(CALENDER_SYSTEM, duration);
		runSystem(WORK_ORDER_HELPER, duration);
	}
	

	// Update systems
	runSystem(MINING_SYSTEM, duration); 
	runSystem(MOVEMENT_SYSTEM, duration);
	runSystem(DIJKSTRA_MAPS_HANDLER, duration);

	runSystem(AI_WORK_SYSTEM, duration);

	// Perform assigned jobs
	runSystem(WORK_ORDERS_SYSTEM, duration);
	runSystem(MINING_AI, duration);
	runSystem(WOODCUTTING_AI, duration);
	runSystem(BUILD_AI, duration);
	runSystem(AI_ARCHITECTURE, duration);
	runSystem(HARVEST_AI, duration);


	runSystem(STOCKPILE_SYSTEM, duration);
	runSystem(HAULING_SYSTEM, duration);
	runSystem(PLANT_SYSTEM, duration);

	static double cameraRefresh = 0.0;
	cameraRefresh += duration;

	// Gui Systems
	if (cameraRefresh > 30.0)
	{
		cameraRefresh = 0.0;
		runSystem(CAMERA_SYSTEM, duration);
	} 

	// Main Rendering 
	runSystem(RENDER_SYSTEM, duration);

	// Menu and gui rendering
	runSystem(MENU_BAR, duration); // Should these be before world.refresh()?

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
	}

	// Update all traits in entities that were killed, deactivated,
	// or activated (when a component changes)
	world.refresh();
}


} // End namespace


