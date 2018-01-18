#pragma once
#include "../../Component.h"
#include "../../../Coordinates.h"

struct LumberjacTag : public Component
{
	LumberjacTag() = default;

	enum ChoppingSteps
	{
		GET_AXE,
		FIND_TREE,
		GOTO_SITE,
		CHOP,
		DROP_AXE
	};

	ChoppingSteps step = GET_AXE;

	std::size_t current_axe = 0;

	std::size_t treeId = 0;

	Coordinates treeCo;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(step, current_axe, treeId, treeCo);
	}
};

CEREAL_REGISTER_TYPE(LumberjacTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, LumberjacTag);