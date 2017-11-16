#pragma once
#include "../Systems.h"
#include "../Components/MovementComponent.h"
#include "../Components/PositionComponent.h"
#include "../../Engine.h"
#include "../../Map.h"

class MovementSystem : public System<Requires<MovementComponent, PositionComponent>>
{
public:
	MovementSystem() = default;

	void update(double tStep)
	{
		auto& entities = getEntities();

		for (auto& e : entities)
		{
			auto& moveComp = e.getComponent<MovementComponent>();
			auto& co = e.getComponent<PositionComponent>().co;

			if (moveComp.progress == 0 && moveComp.controlledMovement && canMoveDir(co))
				updatePos(e);

			else if (moveComp.progress != 0)
				updatePos(e);
		}
	}

private:

	void updatePos(Entity & e)
	{

	}

	// Check that Entity can move into the square
	// so we avoid people walking through walls
	inline bool canMoveDir(Coordinates co)
	{
		return engine.map->tileManager.canPass(co);
	}
};