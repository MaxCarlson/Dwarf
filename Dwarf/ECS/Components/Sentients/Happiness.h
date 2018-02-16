#pragma once
#include "ECS\Component.h"

constexpr double MAX_HAPPINESS = 1000.0;

struct Thought {};

struct Happiness : public Component
{

	double total = 750.0;

	std::vector<Thought> recentThoughts;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(total);
	}
};

CEREAL_REGISTER_TYPE(Happiness);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Happiness);