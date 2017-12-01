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

	void followMap(Entity e, std::vector<int> map)
	{

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