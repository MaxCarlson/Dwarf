#pragma once
#include "../Systems.h"
#include "../Coordinates.h"

struct designation_message : public base_message_t
{
	enum DesignationType
	{
		NONE,
		MINING,
		CHANNELING,
		RAMPING,
		UP_STAIRS,
		DOWN_STAIRS,
		UP_DOWN_STAIRS
	};
	designation_message() = default;
	designation_message(std::pair<int, int> desIdx, uint8_t type) : desIdx(desIdx), type(type) {}

	std::pair<int, int> desIdx;

	uint8_t type = MINING;
};