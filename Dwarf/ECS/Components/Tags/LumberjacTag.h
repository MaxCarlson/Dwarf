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

	ChoppingSteps step;
};