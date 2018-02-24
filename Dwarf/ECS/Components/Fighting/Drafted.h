#pragma once
#include "ECS\Component.h"
#include "Coordinates.h"

struct Drafted : public Component
{
	enum Steps
	{
		STAND,
		GOTO_DEST,
		ATTACK
	};

	int step = STAND;

	Coordinates dest;

	size_t targetId;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(step, dest, targetId);
	}
};

CEREAL_REGISTER_TYPE(Drafted);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Drafted);