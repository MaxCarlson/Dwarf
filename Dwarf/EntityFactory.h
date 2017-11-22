#pragma once

class Entity;
struct Coordinates;

// Used for creating a variety of Entities
// Also probably going to be the place for 
// seriallization
class EntityFactory
{
public:
	EntityFactory();

	Entity createDwarf(Coordinates co);
};

