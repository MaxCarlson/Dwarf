#pragma once
#include "../Component.h"

struct Claimed : public Component
{
	Claimed() = default;

	template<class Archive>
	void serialize(Archive& archive)
	{
	}
};

CEREAL_REGISTER_TYPE(Claimed);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Claimed);