#pragma once
#include "../../Component.h"

struct AiWorkComponent : public Component
{
	AiWorkComponent() = default;

	template<class Archive>
	void serialize(Archive &archive)
	{

	}
};

CEREAL_REGISTER_TYPE(AiWorkComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, AiWorkComponent);