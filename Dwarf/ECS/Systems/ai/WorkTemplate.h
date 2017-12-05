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
		auto& entitiesAtPos = engine.entityPositionCache->findEntities(co);
		
		for (auto& ent : entitiesAtPos)
		{
			auto& toolPos = ent.getComponent<PositionComponent>().co;

			if (toolPos == co && &ent.getComponent<Item>() && ent.getComponent<Item>().catagory.test(catagory))
			{
				auto& stored = ent.addComponent<ItemStored>();
				stored.eid = e.getId().index;
				stored.rend = ent.getComponent<RenderComponent>();
				ent.removeComponent<RenderComponent>();
				toolPos = EMPTY_COORDINATES;
				ent.activate();

				e.getWorld().emit<MSG>();

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