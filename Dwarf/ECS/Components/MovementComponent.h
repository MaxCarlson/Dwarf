#pragma once
#include "../Component.h"
#include "../../Coordinates.h"
#include <queue>

// Used for filtering Entities that can move
// vs those that can't
// Also holds data about movment type
class MovementComponent : public Component
{
public:
	MovementComponent() : speed(0), progress(0), controlledMovement(true) {};


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

	// Holds the entire path calcualted by MovementAiSystem
	// Direction the entity is moving in is in path[0]
	// Using coordinates for easy offsets 
	// for x, y, and z axies
	// Values should never be below -1 or above 1
	// for individual coordinate values
	std::queue<Coordinates> path;

	// Destination square coordinates
	Coordinates destination;

	// Does the entity have control of their movement?
	// Used so Entitys can be pushed out over areas with no floor
	// etc
	bool controlledMovement;
};