#pragma once
#include "../Component.h"
#include "../../Coordinates.h"

// Used for filtering Entities that can move
// vs those that can't
class MovementComponent : public Component
{
public:
	MovementComponent() = default;

	// How fast is the Entity currently moving?
	int speed;

	// How far this Entity has progressed through
	// a given tile. Once it reaches > 1 it'll pass to the 
	// next tile or farther
	// Possibly make an int if float is perf hog?
	float percentOfTileMovement;

	// Direction
	Coordinates direction;
};