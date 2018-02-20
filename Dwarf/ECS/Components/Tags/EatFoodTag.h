#pragma once
#include "ECS\Component.h"
#include "Coordinates.h"

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

	enum TableChairStatus
	{
		BOTH,
		TABLE,
		CHAIR,
		NONE
	};

	size_t foodId = 0;
	size_t tableId = 0;

	Coordinates targetCo;

	int step = FIND_FOOD;

	TableChairStatus tableStatus = NONE;

	double time = 0.0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(foodId, tableId, targetCo, step, tableStatus, time);
	}
};

CEREAL_REGISTER_TYPE(EatFoodTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, EatFoodTag);