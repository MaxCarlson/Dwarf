#pragma once
#include "../../Component.h"

struct AiWorkComponent : public Component
{
	AiWorkComponent() = default;

	std::unordered_map<std::string, int> jobPrefrences;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(jobPrefrences);
	}
};

CEREAL_REGISTER_TYPE(AiWorkComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, AiWorkComponent);