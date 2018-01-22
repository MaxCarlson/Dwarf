#pragma once
#include "../Systems.h"
#include "../Coordinates.h"

static const std::vector<std::string> designateNames =
{
	"Mining", "Channeling", "Ramps", "Up Stairs", "Down Stairs", "Up Down Stairs",
	"Cut Trees", "Harvest"
};

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
		CUT_TREES,
		HARVEST
	};

	designation_message() = default;
	designation_message(std::pair<int, int> desIdx, uint8_t type) : desIdx(desIdx), type(type) {}

	std::pair<int, int> desIdx;

	uint8_t type = MINING;
};