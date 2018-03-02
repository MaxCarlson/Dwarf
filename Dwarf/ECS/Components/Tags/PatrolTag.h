#pragma once
#include "ECS\Component.h"

struct PatrolTag : public Component
{
	enum Steps
	{
		FIND_ROUTE,
		GOTO_ROUTE,
		FOLLOW_ROUTE
	};

	int step = FIND_ROUTE;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(step);
	}
};

CEREAL_REGISTER_TYPE(PatrolTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PatrolTag);