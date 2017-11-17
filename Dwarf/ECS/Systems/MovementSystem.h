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

	void update(float tStep)
	{
		const auto& entities = getEntities();

		for (const auto& e : entities)
		{
			auto& moveComp = e.getComponent<MovementComponent>();
			auto& co = e.getComponent<PositionComponent>().co;

			if (moveComp.progress == 0 && moveComp.controlledMovement && canMoveDir(co))
				updatePos(tStep, e);

			// If movement has already progressed, no need to 
			// check for safety of next square
			else if (moveComp.progress != 0)
				updatePos(tStep, e);
		}
	}

private:

	void updatePos(float tStep, const Entity & e)
	{
		auto& mov = e.getComponent<MovementComponent>();

		mov.progress += tStep / 1000;

		if (mov.progress >= 1.0)
		{
			mov.progress -= 1.0;

			auto& co = e.getComponent<PositionComponent>().co;

			co += mov.direction;

			//mov.direction = { 0, 0, 0 };
		}
	}

	// Check that Entity can move into the square
	// so we avoid people walking through walls
	inline bool canMoveDir(Coordinates co)
	{
		return engine.map->tileManager.canPass(co);
	}

	void initialize() {};

	void onEntityAdd(Entity & entity) {};

	void onEntityRemoval(Entity & entity) {};
};