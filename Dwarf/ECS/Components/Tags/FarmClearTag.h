#pragma once
#include "ECS\Component.h"

struct FarmClearTag : public Component
{
	enum Steps
	{
		TEST_TOOL,
		FIND_HOE,
		FIND_FARM,
		GOTO_FARM,
		CLEAR_AREA
	};

	int dest = 0;
	size_t toolId = 0;
	int step = TEST_TOOL;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(dest, toolId, step);
	}
};
CEREAL_REGISTER_TYPE(FarmClearTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, FarmClearTag);
