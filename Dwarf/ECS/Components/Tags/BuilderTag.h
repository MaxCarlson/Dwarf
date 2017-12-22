#pragma once
#include "../../Component.h"
#include "../helpers/building_designation.h"
#include "../cereal/types/utility.hpp"

struct BuilderTag : public Component
{
	BuilderTag() = default;

	enum build_steps
	{
		FIND_BUILDING,
		FIND_COMPONENT,
		GOTO_COMPONENT,
		GRAB_COMPONENT,
		GOTO_BUILDING,
		ADD_COMPONENT,
		BUILD_BUILDING,
		DROP_COMPONENT
	};

	build_steps step = FIND_BUILDING;

	building_designation buildingTarget;

	std::size_t current_component;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(step, buildingTarget, current_component);
	}
};


CEREAL_REGISTER_TYPE(BuilderTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, BuilderTag);