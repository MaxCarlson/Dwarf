#pragma once
#include "ECS\Component.h"

class FarmSoilTag : public Component
{
	enum Steps
	{
		TEST_TOOL,
		FIND_FARM,
		FIND_HOE,
		GOTO_HOE,
		FIND_SOIL,
		GOTO_SOIL,
		SPREAD_SOIL
	};

	int step = TEST_TOOL;

	int farmIdx = 0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(step, farmIdx);
	}
};
CEREAL_REGISTER_TYPE(FarmSoilTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, FarmSoilTag);
