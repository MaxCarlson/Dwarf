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
		CHOP
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