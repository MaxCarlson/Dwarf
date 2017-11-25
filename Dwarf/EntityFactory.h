#pragma once
#include "Coordinates.h"
#include <string>
#include <vector>

class Entity;
struct DwarfCreationObj;

// Used for creating a variety of Entities
// Also probably going to be the place for 
// seriallization
class EntityFactory
{
public:
	EntityFactory();

	Entity createDwarf(DwarfCreationObj dwarfConstruct);
};

struct DwarfCreationObj
{
	// Dwarf Location
	Coordinates co;

	// Dwarf skill level and dwarf
	// skill points
	//std::vector<int> laborStats;
	std::vector<int> skillPoints;

	// Not in use yet
	std::string name;
};
