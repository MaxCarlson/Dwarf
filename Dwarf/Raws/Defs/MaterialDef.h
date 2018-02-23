#pragma once
#include <string>
#include <vector>

// Structure for holding info read from lua files
// on materials. Will have a bunch more properties once
// more systems are up and running

enum MaterialType
{
	MAT_NONE,
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

	std::string color; // TODO: switch block to vchars
	uint16_t charCode; 
	uint16_t floorCode;
	int tilesetKey;

	// Where is this material found
	std::string layer = "";

	MaterialType matType;
	int health = 1;
	std::string minesToTag = "";
	std::string oreProduct; // Want to add multiple at some point?
	int minesToAmount = 0; // Get rid of this?

	double dmgMultiplier = 1.0; // TODO: Change damage types for different weapons 
	double speedMultiplier = 1.0;
	double armourBonus = 1.0; // TODO: Make armour for different kinds of attacks
};