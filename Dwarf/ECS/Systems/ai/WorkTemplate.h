#pragma once

#include "../../Component.h"
#include "../../World.h"
#include "../../Components/PositionComponent.h"
#include "../../Components/MovementComponent.h"
#include "../../Components/ItemStored.h"
#include "../../Systems/DijkstraSystems/DijkstraMapsHandler.h"

#include "../../../Engine.h"
#include "../../Components/Item.h"
#include "../EntityPositionCache.h"
#include "../../Messages/pickup_item_message.h"
#include "../../Components/Sentients/Inventory.h"

template<typename TAG>
class WorkTemplate
{
public:

	template<typename MSG, typename CANCEL, typename SUCCESS>
	void pickup_tool(const Entity& e, Coordinates co, const int &catagory, std::size_t& out_tool, const CANCEL &cancel, const SUCCESS &success)
	{
		// Entities current tool
		// ( Possibly no tool )
		out_tool = e.getComponent<Inventory>().inventory[SLOT_TOOL];

		// If entity already has the correct tool, we don't need to look
		// for one
		if (out_tool && e.getWorld().getEntity(out_tool).getComponent<Item>().catagory.test(catagory))
		{
			success();
			return;
		}

		// Load all Entities at positon from cache
		auto& entitiesAtPos = engine->entityPositionCache->findEntities(co);
			
		// Entity doesn't have correct tool
		// For all Entities at this position
		for (auto& item : entitiesAtPos)
		{
			auto& pos = item.getComponent<PositionComponent>().co;
			auto* tool = &item.getComponent<Item>();

			// If position cache is wrong, or Entity doesn't have an
			// Item component, skip it
			if (!tool || pos != co)
				continue;

			// Test and make sure it's the proper tool
			if (tool->catagory.test(catagory))
			{
				// Pickup tool if it matches catagory
				// dump old tool in tool slot if it exists
				e.getWorld().emit(pickup_item_message{SLOT_TOOL , e.getId().index, item.getId().index, out_tool});
				e.getWorld().emit(MSG{});

				// Set current tool to tool being picked up
				out_tool = item.getId().index;

				success();
				return;
			}
		}
		cancel();
	}

	template<typename CANCEL, typename SUCCESS>
	void followMap(DijkstraMap &map, const Entity& e, Coordinates co, const CANCEL &cancel, const SUCCESS &success)
	{
		const auto dist = map.get(getIdx(co));

		if (dist > MAX_DIJKSTRA_DISTANCE - 1)
			cancel();

		else if (dist < 1)
			success();
		else
		{
			auto& mov = e.getComponent<MovementComponent>();

			// Don't interrupt progress
			if (mov.progress)
				return;
			else
			{
				const auto destination = map.findDestination(co);
				mov.destination = destination;
				//mov.path.push_back(destination);
				return;
			}
		}
	}

	template<typename CANCEL, typename SUCCESS>
	void followPath(const Entity& e, Coordinates co, Coordinates dest, const CANCEL &cancel, const SUCCESS &success)
	{

	}

	inline void cancel_work(Entity e)
	{
		e.removeComponent<TAG>();
		e.activate();
		// status marker for idle here?
	}

};