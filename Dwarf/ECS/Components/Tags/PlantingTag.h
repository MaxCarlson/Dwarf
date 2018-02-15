#pragma once
#include "../../Component.h"
#include "Coordinates.h"

struct PlantingTag : public Component
{

	enum steps
	{
		FIND_HOE,
		PICKUP_HOE,
		FIND_SEEDS,
		GOTO_SEEDS,
		FIND_PLANTING,
		PLANT,
	};

	int step = FIND_HOE;

	size_t itemId = 0;

	Coordinates targetCo;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(step, itemId, progress, targetCo);
	}
};

CEREAL_REGISTER_TYPE(PlantingTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PlantingTag);
