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

	int deciduosChance = 0;
	int evergreenChance = 0;
};