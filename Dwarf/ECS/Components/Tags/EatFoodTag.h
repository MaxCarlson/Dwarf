#pragma once
#include "ECS\Component.h"

struct EatFoodTag : public Component
{
	enum Steps
	{
		FIND_FOOD,
		GOTO_FOOD,
		FIND_TABLE,
		GOTO_TABLE,
		EAT_FOOD
	};

	size_t foodId = 0;

	int step = FIND_FOOD;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(foodId, step);
	}
};

CEREAL_REGISTER_TYPE(EatFoodTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, EatFoodTag);