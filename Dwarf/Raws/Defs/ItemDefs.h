#pragma once
#include "Drawing/vchar.h"
#include <string>
#include <bitset>

constexpr int NUMBER_OF_ITEM_CATEGORIES = 64;

constexpr int COMPONENT = 0;
constexpr int TOOL_DIGGING = 1;
constexpr int TOOL_CHOPPING = 2;
constexpr int TOOL_FARMING = 3;
constexpr int ITEM_FOOD = 4;
constexpr int ITEM_SEED = 5;
constexpr int ITEM_SOIL = 6;


struct StockpileDef
{
	int index = 0;
	std::string name = "";
	std::string tag = "";
};

struct ItemDef
{
	std::string name = "";
	std::string tag = "";
	std::string description = "";
	std::bitset<NUMBER_OF_ITEM_CATEGORIES> categories;

	vchar ch;
	
	int stackSize = 1;
	int stockpileId = 0;
};