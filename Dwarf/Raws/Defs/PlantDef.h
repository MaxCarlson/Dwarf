#pragma once
#include "stdafx.h"
#include "../Drawing/vchar.h"

constexpr int PLANT_SPREADS = 0;
constexpr int PLANT_ANNUAL = 1;

struct PlantDef
{
	std::string name = "";
	std::string tag = "";

	std::vector<vchar> chars;
	std::vector<int> lifecycle;

	std::vector<std::string> harvestsTo;
	std::bitset<2> tags;
};