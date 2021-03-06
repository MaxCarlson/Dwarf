#pragma once
#include <string>
//#include "../ECS/Component.h"

enum MaterialDefSpawnType
{
	spawn_type_none,
	spawn_type_rock,
	spawn_type_soil,
	spawn_type_sand,
	spawn_type_metal,
	spawn_type_synthetic,
	spawn_type_organic
};

struct ReactionInput
{
	std::string tag = "";
	std::size_t req_material = 0;

	MaterialDefSpawnType req_material_type = spawn_type_none;

	int quantity = 0;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(tag, req_material, req_material_type, quantity);
	}
};

//CEREAL_REGISTER_TYPE(ReactionInput);