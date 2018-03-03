#pragma once
#include "ECS\Component.h"

// This tag works a little differently than others
// in that it's main purpose is just to filter military
// personel out from non military
// It doesn't just act as a job distrubtor agent like the others do
// and can be gained from places other than a single descrete system
//
// Sub - Tags such as PatrolTag are used for filtering entities into 
// specific military jobs/roles

static const std::vector<std::string> militaryStateNames = { "Off Duty", "Patroling", "Guarding", "Attacking" };

struct MilitaryTag : public Component
{
	MilitaryTag() = default;
	MilitaryTag(const std::string &squadTag) : squadTag(squadTag) {}

	enum Job
	{
		OFF_DUTY,
		PATROL,
		GUARD,
		ATTACK
	};

	std::string squadTag = "";

	int currentJob = OFF_DUTY;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(squadTag, currentJob);
	}
};

CEREAL_REGISTER_TYPE(MilitaryTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, MilitaryTag);