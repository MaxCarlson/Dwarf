#pragma once
#include "ECS\Component.h"

class FarmSoilTag : public Component
{
	enum Steps
	{
		FIND_FARM,
		FIND_HOE,
		GOTO_HOE,
		GOTO_SOIL,
		SPREAD_SOIL
	};

	int step = FIND_FARM;

	int farmIdx = 0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(step, farmIdx);
	}
};
CEREAL_REGISTER_TYPE(FarmSoilTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, FarmSoilTag);
