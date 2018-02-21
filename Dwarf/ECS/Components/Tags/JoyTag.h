#pragma once
#include "ECS\Component.h"
#include "Coordinates.h"

struct JoyTag : public Component
{
	JoyTag() = default;

	enum Steps
	{
		FIND_SOURCE,
		GOTO_SOURCE,
		PLAY
	};

	size_t sourceId = 0;
	Coordinates sourceCo;

	int step = FIND_SOURCE;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(sourceId, sourceCo, step);
	}
};

CEREAL_REGISTER_TYPE(JoyTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, JoyTag);