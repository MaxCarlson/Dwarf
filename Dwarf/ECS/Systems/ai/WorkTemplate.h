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

template<typename TAG>
class WorkTemplate
{
public:

	template<typename MSG, typename CANCEL, typename SUCCESS>
	void pickup_tool(const Entity& e, Coordinates co, const int &catagory, const CANCEL &cancel, const SUCCESS &success)
	{
		// Load all Entities at positon from cache
		auto& entitiesAtPos = engine.entityPositionCache->findEntities(co);
		
		std::size_t tool_id = 0;

		for (auto& ent : entitiesAtPos)
		{
			auto& pos = ent.getComponent<PositionComponent>().co;
			auto* tool = ent.getComponent<Item>();

			// If position cache is wrong, or Entity doesn't have an
			// Item component, skip it
			if (pos != co || !tool)
				continue;

			// Test and make sure it's the proper tool
			if (tool.catagory.test(catagory))
			{





				auto& stored = ent.addComponent<ItemStored>();
				stored.eid = e.getId().index;
				stored.rend = ent.getComponent<RenderComponent>();
				ent.removeComponent<RenderComponent>();
				toolPos = EMPTY_COORDINATES;
				ent.activate();

				e.getWorld().emit(MSG{});

				success();
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
				return;
			}
		}
	}

	inline void cancel_work(Entity e)
	{
		e.removeComponent<TAG>();
		e.activate();
		// status marker for idle here?
	}

};