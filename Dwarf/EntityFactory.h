#pragma once

class Entity;
class Coordinates;

// Used for creating a variety of Entities
// Also probably going to be the place for 
// seriallization
class EntityFactory
{
public:
	EntityFactory();

	// Create a camera, done only once right now
	// with local world generation
	Entity createCamera(int screenWidth, int screenHeight);

	Entity createDwarf(Coordinates co);
};

