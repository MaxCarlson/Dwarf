#pragma once

#include "../../Component.h"
#include "../../Coordinates.h"

class MiningTag : public Component
{
public:
	MiningTag() = default;

	enum mining_steps
	{
		GET_PICK,
		GOTO_SITE,
		DIG,
		DROP_TOOL
	};

	mining_steps step = GET_PICK;

	std::size_t currentPick = 0;
};