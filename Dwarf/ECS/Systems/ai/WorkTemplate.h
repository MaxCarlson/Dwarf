#pragma once

#include "../../Component.h"
#include "../../World.h"
#include "../../Components/PositionComponent.h"
#include "../../Components/MovementComponent.h"
#include "../../Systems/DijkstraSystems/DijkstraMapsHandler.h"

template<typename TAG>
class WorkTemplate
{
public:


	void pickup_tool(Entity& e)
	{
		return;

		auto& mov = e.getComponent<MovementComponent>();

		//mov.destination = tool_location;
	}

	template<typename CANCEL, typename SUCCESS>
	void followMap(DijkstraMap &map, Entity e, Coordinates co, const CANCEL &cancel, const SUCCESS &success)
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

	bool followPath(Entity& e, TAG & tag)
	{


		return false;
	}

	inline void cancel_work(Entity & e)
	{
		e.removeComponent<TAG>();
		e.activate();
		// status marker for idle here?
	}

};