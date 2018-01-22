#pragma once
#include "../../Component.h"

struct HarvestTag : public Component
{
	enum HarvestSteps
	{
		FIND_HARVEST,
		HARVEST
	};

	HarvestSteps step = FIND_HARVEST;

	int idx = 0;

	double progress = 0.0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(step, plantId, idx, progress);
	}
};
CEREAL_REGISTER_TYPE(HarvestTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, HarvestTag);