#pragma once
#include "ECS\Component.h"

struct Name : public Component
{
	Name() = default;
	Name(const std::string &first, const std::string &last) 
				: firstName(first), lastName(last) {}

	std::string firstName = "";
	std::string lastName = "";

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(firstName, lastName);
	}
};

CEREAL_REGISTER_TYPE(Name);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Name);