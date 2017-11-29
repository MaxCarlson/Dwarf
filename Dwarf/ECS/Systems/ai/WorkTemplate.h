#pragma once

#include "../../Component.h"
#include "../../World.h"
#include "../../Components/PositionComponent.h"
#include "../../Components/MovementComponent.h"

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

	bool followPath(Entity& e, TAG & tag)
	{
		auto& pos = e.getComponent<PositionComponent>();
		auto& mov = e.getComponent<MovementComponent>();

		if (mov.destination == EMPTY_COORDINATES)
			mov.destination = tag.desination;

		// Entity is at site!
		if (pos.co == mov.destination)
			return true;

		return false;
	}

	inline void cancel_work(Entity & e)
	{
		e.removeComponent<TAG>();

		// status marker for idle here?
	}

};