#pragma once
#include "ECS\Component.h"

constexpr double MAX_HAPPINESS = 1000.0;

struct Happiness : public Component
{

	double total = 500.0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(total);
	}
};

CEREAL_REGISTER_TYPE(Happiness);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Happiness);