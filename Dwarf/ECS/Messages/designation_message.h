#pragma once
#include "../Systems.h"
#include "../Coordinates.h"

struct designation_message : public base_message_t
{
	enum DesignationType
	{
		MINING,
		CHANNELING,
		RAMPING,
		UP_STAIRS,
		DOWN_STAIRS,
		UP_DOWN_STAIRS,
		CUT_TREES, // Possible move these into their respective GUI files
		HARVEST
	};

	designation_message() = default;
	designation_message(std::pair<int, int> desIdx, uint8_t type) : desIdx(desIdx), type(type) {}

	std::pair<int, int> desIdx;

	uint8_t type = MINING;
};