#pragma once
#include "stdafx.h"

struct BodyPart // Move this, this will be inside entity
{
	int idx = 0; // Where in this species body def vector is this parts def located?
	std::string tag; 

	double health = 1.0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(idx, tag, health);
	}
};

namespace BodyPartEffects
{
	constexpr int ON_ALL_GONE_KILL = 0;
	constexpr int DAMAGE_MOBILITY  = 1;
}
constexpr int MAX_BODY_EFFECTS = 2;

struct BodyPartDef
{
	std::string tag = "";
	std::string name = "";
	std::string description = "";

	double health = 1.0; // TODO: Make this relative to species?

	std::bitset<MAX_BODY_EFFECTS> effects;
};

// These are used to define the layout of body parts
// that make up a species body
struct SpeciesBodyPart
{
	int id = 0;
	std::string tag = "";
	std::string attachedTo = "";
	int idAttachedTo = 0;

	// How likely this is to be hit
	double size = 0.0;
};

struct BodyDef
{
	std::string tag = "";

	// Roll between 0.0 and this number
	// then scan for the first hitChance with a higher value,
	// and use that idx - 1 to damage the part in partGroups 
	double maxChance = 0.0;

	std::vector<double> hitChances;
	std::vector<SpeciesBodyPart> parts;
};
