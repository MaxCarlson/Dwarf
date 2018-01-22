#pragma once
#include "../Drawing/vchar.h"
#include <string>

struct BiomeDef
{
	std::string name = "";

	int minRain = 0;
	int maxRain = 100;
	int minTemp = -100;
	int maxTemp = 100;

	int soil = 100;
	int sand = 0;

	// Only going to be used in world view
	vchar glyph;

	// What terrain type does this biome pop up in?
	std::vector<int> occurs; 

	//plants and their frequency
	std::vector<std::pair<std::string, int>> plants;

	int deciduosChance = 0;
	int evergreenChance = 0;
};