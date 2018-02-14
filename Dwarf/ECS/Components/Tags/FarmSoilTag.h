#pragma once
#include "ECS\Component.h"
#include "Coordinates.h"

struct FarmSoilTag : public Component
{
	enum Steps
	{
		FIND_SOIL,
		GOTO_SOIL,
		FIND_FARM,
		GOTO_FARM,
		SPREAD_SOIL
	};

	int step = FIND_SOIL;

	size_t soilId = 0;
	Coordinates soilCo;
	Coordinates farmCo;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(step, soilId, soilCo, farmCo);
	}
};
CEREAL_REGISTER_TYPE(FarmSoilTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, FarmSoilTag);
