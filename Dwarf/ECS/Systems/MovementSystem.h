#pragma once
#include "../Systems.h"
#include "../Components/MovementComponent.h"
#include "../Components/PositionComponent.h"
#include "../../Engine.h"
#include "../Map/Map.h"
#include "../Messages/entity_moved_message.h"
#include <queue>

class MovementSystem : public System<Requires<MovementComponent, PositionComponent>>
{
public:
	MovementSystem() = default;

	void update(double tStep)
	{
		const auto& entities = getEntities();

		for (const auto& e : entities)
		{
			auto& moveComp = e.getComponent<MovementComponent>();
			auto& co = e.getComponent<PositionComponent>().co;

			// Should we do safety checks here or only if map is updated?
			// Possibly re calculate path if Entities bump into eachother??
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
	void updatePos(double tStep, const Entity & e)
	{
		auto& mov = e.getComponent<MovementComponent>();

		// Update progress by moveSpeed * timeStep,
		// in seconds
		mov.progress += (tStep * mov.speed / 1000);

		// If progress has passed 1.0, 
		// it's time to move to next tile

		// This needs to be 1.44 for diagonal movement!!!
		// Find easy way to calc this
		if (mov.progress >= 1.0)
		{
			// Will this need to be changed eventually?
			mov.progress = 0.0;

			// If the square we're trying to move into is already occupied
			// discard progress and recalcuate path. This needs to be handled better eventually
			//if (tileManager->getProperty<Tile::OBSTRUCTED>(mov.path.back()))
			//{
			//	mov.progress = 0.1;
			//	return;
			//}
				

			auto& co = e.getComponent<PositionComponent>().co;
			Coordinates oldCo = co;
			//tileManager->removeProperty<Tile::OBSTRUCTED>(co);

			// Move Entity to next path coordiantes
			// and pop it from the path vector
			// which is stored in reverse order
			co = mov.path.back();
			mov.path.pop_back();


			// Probably don't want this to be deffered since cache would
			// be wrong sometimes
			emit(entity_moved_message{ e, co, oldCo });
			//emit_deffered(entity_moved_message{ e, co, oldCo });

			//tileManager->setProperty<Tile::OBSTRUCTED>(co);

			// If this Entity has finished following it's path
			// reset it's destination coordinates to none
			if (mov.path.empty())
				mov.destination = EMPTY_COORDINATES;

		}
	}

	// Check that Entity can move into the square
	// so we avoid people walking through walls
	inline bool canMoveDir(Coordinates co)
	{
		return tileManager->canPass(co);
	}
};