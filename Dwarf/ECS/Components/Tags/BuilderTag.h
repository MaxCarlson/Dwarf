#pragma once
#include "../../Component.h"

struct BuilderTag : public Component
{
	BuilderTag() = default;

	enum build_steps
	{
		FIND_BUILDING,
		FIND_COMPONENT,
		GOTO_COMPONENT,
		GRAB_COMPONENT,
		GOTO_BUILDING,
		ADD_COMPONENT,
		BUILD_BUILDING,
		DROP_COMPONENT
	};

	build_steps step = FIND_BUILDING;
};


CEREAL_REGISTER_TYPE(BuilderTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, BuilderTag);