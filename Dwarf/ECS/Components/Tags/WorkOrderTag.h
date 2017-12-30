#pragma once
#include "../../Component.h"
#include "../helpers/work_order_reaction.h"

struct WorkOrderTag : public Component
{
	WorkOrderTag() = default;

	enum work_order_steps
	{
		FIND_WORKSHOP,
		FIND_COMPONENT,
		GOTO_COMPONENT,
		GRAB_COMPONENT,
		DROP_COMPONENT,
		DO_WORK
	};

	work_order_steps step = FIND_WORKSHOP;

	work_order_reaction reaction;

	std::size_t curent_component = 0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(step, reaction, curent_component);
	}
};


CEREAL_REGISTER_TYPE(WorkOrderTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, WorkOrderTag);