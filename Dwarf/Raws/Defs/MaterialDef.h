#pragma once
#include <string>

// Structure for holding info read from lua files
// on materials. Will have a bunch more properties once
// more systems are up and running

enum MaterialType
{
	MAT_ROCK,
	MAT_SOIL,
	MAT_SAND,
	MAT_METAL,
	MAT_SYTHETIC,
	MAT_ORGANIC
};

struct MaterialDef
{
	std::string name = "";
	std::string tag = "";
	std::string description = "";

	std::string color;
	uint16_t charCode;
	int tilesetKey;

	MaterialType matType;
	int health = 1;
	std::string minesToTag = "";
	int minesToAmount = 0;
};