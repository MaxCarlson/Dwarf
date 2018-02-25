#pragma once
#include "ECS\Component.h"
#include "Coordinates.h"

struct Drafted : public Component
{
	enum Steps
	{
		DEFEND,
		GOTO_DEST,
		GOTO_TARGET,
		ATTACK
	};

	int step = DEFEND;

	// This is stored here so we know when we have moved a tile
	// in order to update pathing to target
	Coordinates myCo;
	// Likewise stored so we can update path if focused 
	// on moving target when necassary
	Coordinates targetCo;

	size_t targetId;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(myCo, targetCo, targetId);
	}
};

CEREAL_REGISTER_TYPE(Drafted);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Drafted);