#include "stdafx.h"
#include "RunSystems.h"
#include "Engine.h"
#include "ECS\Systems.h"
// Systems
#include "ECS\Systems\RenderSystem.h"
#include "ECS\Systems\MovementSystem.h"
#include "ECS\Systems\ai\MovementAiSystem.h"
#include "ECS\Systems\MiningSystem.h"
#include "ECS\Systems\DijkstraSystems\DijkstraMapsHandler.h"
#include "ECS\Systems\EntityPositionCache.h"
#include "ECS\Systems\Gui\InputHandler.h"

// Ai systems
#include "ECS\Systems\ai\EquipHandler.h"
#include "ECS\Systems\ai\AiWorkSystem.h"
#include "ECS\Systems\ai\MiningAi.h"
#include "ECS\Systems\ai\BuildAi.h"
#include "ECS\Systems\ai\WorkOrders.h"
#include "ECS\Systems\ai\StockpileSystem.h"
#include "ECS\Systems\ai\HaulingSystem.h"

// System Helpers
#include "ECS\Systems\helpers\ItemHelper.h"
#include "ECS\Systems\helpers\DesignationHandler.h"
#include "ECS\Systems\helpers\WorkOrderHelper.h"

constexpr int RENDER_SYSTEM = 0;
constexpr int MOVEMENT_SYSTEM = 1;
constexpr int MOVEMENT_AI_SYSTEM = 2;
constexpr int AI_WORK_SYSTEM = 3;
constexpr int MINING_AI = 4;
constexpr int BUILD_AI = 5;
constexpr int WORK_ORDERS = 6;
constexpr int STOCKPILE_SYSTEM = 7;
constexpr int HAULING_SYSTEM = 8;
constexpr int INPUT_HANDLER = 9;
constexpr int DESIGNATION_HANDLER = 10;
constexpr int MINING_SYSTEM = 11;
constexpr int DIJKSTRA_MAPS_HANDLER = 12;
constexpr int ENTITY_POSITION_CACHE = 13;
constexpr int EQUIP_HANDLER = 14;
constexpr int ITEM_HELPER = 15;
constexpr int WORK_ORDER_HELPER = 16;

namespace RunSystems
{
	boost::container::flat_map<int, SystemBase*> systems;

	World* world;

	void initSystems()
	{
		world = &engine->world;
		
		world->addSystem(*(systems[INPUT_HANDLER] = new InputHandler));
		world->addSystem(*(systems[DESIGNATION_HANDLER] = new DesignationHandler));
		world->addSystem(*(systems[RENDER_SYSTEM] = new RenderSystem));
		world->addSystem(*(systems[MOVEMENT_SYSTEM] = new MovementSystem));
		world->addSystem(*(systems[MOVEMENT_AI_SYSTEM] = new MovementAiSystem));
		world->addSystem(*(systems[MINING_SYSTEM] = new MiningSystem));
		world->addSystem(*(systems[AI_WORK_SYSTEM] = new AiWorkSystem));
		world->addSystem(*(systems[MINING_AI] = new MiningAi));
		world->addSystem(*(systems[DIJKSTRA_MAPS_HANDLER] = new DijkstraMapsHandler));
		world->addSystem(*(systems[ENTITY_POSITION_CACHE] = new EntityPositionCache));
		world->addSystem(*(systems[EQUIP_HANDLER] = new EquipHandler));
		world->addSystem(*(systems[BUILD_AI] = new BuildAi));
		world->addSystem(*(systems[ITEM_HELPER] = new ItemHelper));
		world->addSystem(*(systems[WORK_ORDERS] = new WorkOrders));
		world->addSystem(*(systems[WORK_ORDER_HELPER] = new WorkOrderHelper));
		world->addSystem(*(systems[STOCKPILE_SYSTEM] = new StockpileSystem));
		world->addSystem(*(systems[HAULING_SYSTEM] = new HaulingSystem));

		for (auto& sys : systems)
			sys.second->init();

		world->refresh();
	}

	template<typename T>
	void runSystem()
	{

	}

	void updateSystems(const double duration)
	{

	}

	void killSystems()
	{
		world->removeAllSystems();
	}
}


