#pragma once
#include "../Systems.h"
#include "../Components/MovementComponent.h"
#include "../Components/PositionComponent.h"
#include "../../Engine.h"
#include "../../Map.h"
#include <queue>

class MovementSystem : public System<Requires<MovementComponent, PositionComponent>>
{
public:
	MovementSystem() = default;

	void update(float tStep)
	{
		const auto& entities = getEntities();

		for (const auto& e : entities)
		{
			auto& moveComp = e.getComponent<MovementComponent>();
			auto& co = e.getComponent<PositionComponent>().co;

			// Should we do safety checks here or only if map is updated?
			//Coordinates newCords = co + moveComp.direction;
		
			if (!moveComp.path.empty())
				updatePos(tStep, e);
		}
	}

private:

	// Updates position of Entity.
	// If the Entity completes movement through a square,
	// zero their movement direction, then let MovementAI
	// know to do more pathfinding. Could be better to hold a vector of movement squares in 
	// movement component, and only change it if the map changes!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	bool updatePos(float tStep, const Entity & e)
	{
		auto& mov = e.getComponent<MovementComponent>();

		mov.progress += tStep / 1000;

		// If progress has passed 1.0
		// it's time to move to next tile
		if (mov.progress >= 1.0)
		{
			// Will this need to be changed eventually?
			mov.progress = 0.0;

			auto& co = e.getComponent<PositionComponent>().co;

			// Add coordinate movement to Entity
			// coordiantes and pop it from the path vector
			// which is stored in reverse order
			co = mov.path.back();
			mov.path.pop_back();

			// If this Entity has finished following it's path
			// reset it's destination coordinates to none
			if (mov.path.empty())
				mov.destination = EMPTY_COORDINATES;

			return true;
		}

		return false;
	}

	// Check that Entity can move into the square
	// so we avoid people walking through walls
	inline bool canMoveDir(Coordinates co)
	{
		return engine.map->tileManager.canPass(co);
	}
};