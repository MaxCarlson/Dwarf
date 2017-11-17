#pragma once
#include "../Component.h"
#include "../../Coordinates.h"

// Used for filtering Entities that can move
// vs those that can't
// Also holds data about movment type
class MovementComponent : public Component
{
public:
	MovementComponent() : speed(0), progress(0), direction({ 0, 0, 0 }), controlledMovement(true) {};
	MovementComponent(int speed, float progress, Coordinates direction, bool controlledMovement) 
		          : speed(speed), progress(progress), direction(direction), controlledMovement(controlledMovement) {};

	// How fast is the Entity currently moving?
	// For very simple use we could make this into 1000 default
	// representing 1 square per second move speed
	// with 100 being 10 squares per second
	int speed;

	// How far this Entity has progressed through
	// a given tile. Once it reaches > 1 it'll pass to the 
	// next tile or farther
	// Possibly make an int if float is perf hog?
	float progress;

	// Direction the entity is moving in
	// Using coordinates for easy offsets 
	// for x, y, and z axies
	// Values should never be below -1 or above 1
	Coordinates direction;

	// Does the entity have control of their movement?
	// Used so Entitys can be pushed out over areas with no floor
	// etc
	bool controlledMovement;
};