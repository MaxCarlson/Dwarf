#pragma once
#include "../Component.h"
#include "../../Tile.h"

// Holds positions of Entities
class PositionComponent : public Component
{
public:
	Coordinates co;

	PositionComponent() = default;
	PositionComponent(Coordinates co) : co(co) {};
	~PositionComponent() {};
};