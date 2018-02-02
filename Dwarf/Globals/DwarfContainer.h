#pragma once
#include <vector>
#include "ECS\Entities.h"

struct DwarfContainer
{
	std::vector<Entity> dwarves;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(dwarves);
	}
};


extern DwarfContainer dwarfContainer;
