#pragma once
#include "../Systems.h"
#include "../Coordinates.h"

struct designation_message : public base_message_t
{
	enum DesignationType
	{
		MINING,
		CHANNELING
	};
	designation_message() = default;
	designation_message(std::pair<int, int> desIdx, int type) : desIdx(desIdx), type(type) {}

	std::pair<int, int> desIdx;

	int type = MINING;
};