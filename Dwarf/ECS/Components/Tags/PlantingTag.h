#pragma once
#include "../../Component.h"

struct PlantingTag : public Component
{

	enum steps
	{
		FIND_SEEDS,
		GOTO_SEEDS,
		FIND_PLANTING,
		PLANT,
	};

	int step = FIND_PLANTING;

	size_t plantDefIdx = 0;

	double progress = 0.0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(step, plantDefIdx, progress);
	}
};
CEREAL_REGISTER_TYPE(PlantingTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PlantingTag);