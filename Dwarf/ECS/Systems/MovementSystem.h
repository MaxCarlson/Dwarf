#pragma once
#include "../Systems.h"
#include "../Components/MovementComponent.h"
#include "../Components/PositionComponent.h"

class MovementSystem : public System<Requires<MovementComponent, PositionComponent>>
{
public:
	MovementSystem() = default;

	void update(double tStep)
	{

	}
};