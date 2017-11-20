#pragma once
#include "../Component.h"
#include "../../Coordinates.h"

// Used for filtering Entities that can move
// vs those that can't
// Also holds data about movment type
class MovementComponent : public Component
{
public:
	MovementComponent() : speed(1), progress(0), destination(EMPTY_COORDINATES) {};
	MovementComponent(double speed) : speed(speed) {};


	// How fast is the Entity currently moving?
	// For very simple use we could make this into 1000 default
	// representing 1 square per second move speed
	// with 100 being 100 squares per second
	// Should this be moved to CreatureStatsComponent? Probably
	double speed;

	// How far this Entity has progressed through
	// a given tile. Once it reaches > 1 it'll pass to the 
	// next tile or farther
	// Possibly make an int if float is perf hog?
	float progress;

	// Destination square coordinates
	Coordinates destination;


	// Holds the entire path calcualted by MovementAiSystem
	// Direction the entity is moving in is in path[0]
	// Using coordinates for easy offsets 
	// for x, y, and z axies
	// Values should never be below -1 or above 1
	// for individual coordinate values
	std::vector<Coordinates> path;
};