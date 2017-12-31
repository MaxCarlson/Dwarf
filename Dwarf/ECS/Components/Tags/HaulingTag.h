#pragma once
#include "../../Component.h"

struct HaulingTag : public Component
{
	HaulingTag() = default;

	enum hauling_steps
	{
		FIND_JOB,
		GOTO_PIKCUP,
		DROP_ITEM
	};

	hauling_steps step = FIND_JOB;

	std::size_t currentItem = 0;

	std::size_t stockpileId = 0;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(step, currentItem, stockpileId);
	}
};

CEREAL_REGISTER_TYPE(HaulingTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, HaulingTag);