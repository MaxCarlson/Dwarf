#pragma once

//#include "../../Component.h"
#include "../../../Coordinates.h"
#include "../../../Raws/ReactionInput.h"

struct building_designation
{
	// top upper left coordinate
	Coordinates co;

	std::string name;
	std::string tag;

	int width;
	int height;

	// Id if this building
	std::size_t entity_id;

	// Keep track of which components are being used for building
	// so we can delete them once done creating the building
	std::vector<std::pair<std::size_t, bool>> componentIds;

	// What do we need to make this building?
	std::vector<ReactionInput> components;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(co, name, tag, width, height, entity_id, componentIds, components);
	}
};


//CEREAL_REGISTER_TYPE(building_designation);
//CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, BuilderTag);