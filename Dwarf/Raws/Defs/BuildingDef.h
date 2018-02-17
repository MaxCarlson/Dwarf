#pragma once
#include <string>
#include <vector>
#include "../ReactionInput.h"

enum b_provides_enum
{
	PROVIDES_WORKSHOP,
	PROVIDES_FLOOR,
	PROVIDES_WALL,
	PROVIDES_RAMP,
	PROVIDES_UP_STAIRS,
	PROVIDES_DOWN_STAIRS,
	PROVIDES_UP_DOWN_STAIRS,
	PROVIDES_SLEEP,
	MAX_BUILDING_PROVIDES
};

struct BuildingProvides
{
	BuildingProvides() = default;
	BuildingProvides(b_provides_enum provides) : provides(provides) {}

	b_provides_enum provides;

	// Not implemented yet
	//int radius = 0;
};

struct BuildingDef
{
	std::string name;
	std::string tag;
	std::string description;

	int width = 1;
	int height = 1;

	// What do we need to make this building?
	std::vector<ReactionInput> components;

	// Any special building attributes?
	std::bitset<MAX_BUILDING_PROVIDES> provides;

	std::pair<std::string, int> skill_required;

	// contains every render character
	// needed starting from the top left
	// to bottom right ~~ possibly make a struct that holds char info as well as forground + background colors?
	std::vector<int> charCodes;

	double buildTime = 1000.0;

	bool structure = false;
};
