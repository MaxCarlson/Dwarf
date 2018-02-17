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

	int step = FIND_BED;

	Coordinates bedCo;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(step, bedCo);
	}
};

CEREAL_REGISTER_TYPE(SleepTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, SleepTag);
