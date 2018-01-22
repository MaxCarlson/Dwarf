#pragma once
#include "../../Component.h"

struct PlantingTag : public Component
{

	template<class Archive>
	void serialize(Archive &archive)
	{
		//archive();
	}
};
CEREAL_REGISTER_TYPE(PlantingTag);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PlantingTag);