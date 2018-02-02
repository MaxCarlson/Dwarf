#include "stdafx.h"
#include "RunSystems.h"
#include "ECS\Systems.h"
#include "Globals\GlobalWorld.h"

#include "ECS\Systems\RenderSystem.h"
#include "ECS\Systems\MovementSystem.h"
#include "ECS\Systems\ai\MovementAiSystem.h"
#include "ECS\Systems\MiningSystem.h"
#include "ECS\Systems\DijkstraSystems\DijkstraMapsHandler.h"

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

// Non systems
#include "ECS\Systems\EntityPositionCache.h"
#include "Globals\global_calender.h"
#include "Designations.h"
#include "Raws\DefInfo.h"

const std::string RENDER_SYSTEM = "Render System";
const std::string MOVEMENT_SYSTEM = "Movement System";
const std::string MOVEMENT_AI_SYSTEM = "Movement Ai System";
const std::string AI_WORK_SYSTEM = "Ai Work System";
const std::string MINING_AI = "Mining Ai";
const std::string BUILD_AI = "Build Ai";
const std::string WORK_ORDERS = "Work Orders";
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

//const std::string WORK_ORDER_HELPER = "Work Order Helper";

namespace RunSystems
{

//boost::container::flat_map<std::string, SystemBase*> systems;
std::unordered_map<std::string, SystemBase*> systems;


void initSystems()
{	
	// External system like Objects that aren't Systems
	positionCache = std::make_unique<PositionCache>();
	calender = std::make_unique<Calender>(); // Eventually give an input date based on world generation
	designations = std::make_unique<Designations>();
	defInfo = std::make_unique<DefInfo>();
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
	systems[WORK_ORDERS] = new WorkOrders;
	systems[WORK_ORDER_HELPER] = workOrderHelper.get();
	systems[STOCKPILE_SYSTEM] = new StockpileSystem;
	systems[HAULING_SYSTEM] = new HaulingSystem;
	systems[AI_ARCHITECTURE] = new AiArchitecture;
	systems[WOODCUTTING_AI] = new WoodcuttingAi;
	systems[HARVEST_AI] = new HarvestAi;
	systems[FARMING_AI] = new FarmingAi;
	systems[CALENDER_SYSTEM] = new CalenderSystem;
	systems[PLANT_SYSTEM] = new PlantSystem;

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
	world.addSystem(* static_cast<WorkOrders *>(systems[WORK_ORDERS]));
	world.addSystem(* static_cast<WorkOrderHelper *>(systems[WORK_ORDER_HELPER]));
	world.addSystem(* static_cast<StockpileSystem *>(systems[STOCKPILE_SYSTEM]));
	world.addSystem(* static_cast<HaulingSystem *>(systems[HAULING_SYSTEM]));
	world.addSystem(* static_cast<AiArchitecture *>(systems[AI_ARCHITECTURE]));
	world.addSystem(* static_cast<WoodcuttingAi *>(systems[WOODCUTTING_AI]));
	world.addSystem(* static_cast<HarvestAi *>(systems[HARVEST_AI]));
	world.addSystem(* static_cast<FarmingAi *>(systems[FARMING_AI]));
	world.addSystem(* static_cast<CalenderSystem *>(systems[CALENDER_SYSTEM]));
	world.addSystem(* static_cast<PlantSystem *>(systems[PLANT_SYSTEM]));

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
	runSystem(WORK_ORDERS, duration);
	runSystem(MINING_AI, duration);
	runSystem(WOODCUTTING_AI, duration);
	runSystem(BUILD_AI, duration);
	runSystem(AI_ARCHITECTURE, duration);
	runSystem(HARVEST_AI, duration);


	runSystem(STOCKPILE_SYSTEM, duration);
	runSystem(HAULING_SYSTEM, duration);
	runSystem(PLANT_SYSTEM, duration);

	// Update all traits in entities that were killed, deactivated,
	// or activated (when a component changes)
	world.refresh();

	runSystem(RENDER_SYSTEM, duration);
}


} // End namespace


