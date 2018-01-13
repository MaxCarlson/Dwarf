#pragma once
#include "../../Component.h"

struct LumberjacTag : public Component
{
	LumberjacTag() = default;

	enum ChoppingSteps
	{
		GET_AXE,
		GOTO_SITE,
		CHOP
	};

	ChoppingSteps step = GET_AXE;

	std::size_t current_axe = 0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(step, current_axe);
	}
};