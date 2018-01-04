#pragma once
#include "../../Component.h"

struct ArchitectTag : public Component
{
	ArchitectTag() = default;

	enum ArchitectSteps
	{
		GOTO_BLOCK,
		GRAB_BLOCK,
		GOTO_SITE,
		BUILD
	};

	// Actually represents the block id,
	// need to use same name for work template
	std::size_t current_tool = 0;

	ArchitectSteps step = GOTO_BLOCK;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(current_tool, step);
	}
};

CEREAL_REGISTER_TYPE(ArchitectTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ArchitectTag);