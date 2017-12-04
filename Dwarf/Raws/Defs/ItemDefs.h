#pragma once
#include <string>
#include <bitset>

constexpr int NUMBER_OF_ITEM_CATEGORIES = 32;
constexpr int TOOL_DIGGING = 0;
constexpr int TOOL_CHOPPING = 1;

struct ItemDef
{
	std::string name = "";
	std::string tag = "";
	std::string description = "";
	std::bitset<NUMBER_OF_ITEM_CATEGORIES> categories;

	uint16_t charCode;
	int tilesetKey;
	int color;
};