#pragma once
#include "ECS\Component.h"
#include "Coordinates.h"

struct SleepTag : public Component
{
	enum Steps
	{
		FIND_BED,
		GOTO_BED,
		SLEEP
	};

	enum BedStatus
	{
		OWN_BED,
		CLAIMED_BED,
		NO_BED
	};

	int step = FIND_BED;

	Coordinates bedCo;

	size_t bedId = 0;
	BedStatus bedStatus = OWN_BED;

	double qualityBenifit = 0.0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(step, bedCo, bedId, bedStatus, qualityBenifit);
	}
};

CEREAL_REGISTER_TYPE(SleepTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, SleepTag);
