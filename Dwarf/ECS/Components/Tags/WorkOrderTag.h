#pragma once
#include "../../Component.h"
#include "../helpers/work_order_reaction.h"
#include "Coordinates.h"

struct WorkOrderTag : public Component
{
	WorkOrderTag() = default;

	enum work_order_steps
	{
		FIND_WORKSHOP,
		FIND_COMPONENT,
		GOTO_COMPONENT,
		GOTO_WORKSHOP,
		WORK_WORKSHOP
	};

	work_order_steps step = FIND_WORKSHOP;

	work_order_reaction reaction;

	std::size_t current_component = 0;

	Coordinates compCo;

	// How far along in the reaction are we?
	double progress = 0.0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(step, reaction, current_component, compCo, progress);
	}
};


CEREAL_REGISTER_TYPE(WorkOrderTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, WorkOrderTag);