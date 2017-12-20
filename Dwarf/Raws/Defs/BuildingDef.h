#pragma once
#include <string>
#include <vector>
#include "../ReactionInput.h"

enum b_provides_enum
{
	provides_floor,
	provides_wall,
	provides_ramp,
};

struct BuildingProvides
{
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

	// What does this building do?
	std::vector<BuildingProvides> provides;

	std::pair<std::string, int> skill_required;

	// contains every render character
	// needed starting from the top left
	// to bottom right
	std::vector<int> charCodes;

	bool structure = false;
};
