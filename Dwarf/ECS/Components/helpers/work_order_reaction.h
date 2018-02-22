#pragma once
#include "cereal.hpp"
#include "../cereal/types/utility.hpp"
#include "Coordinates.h"

struct work_order_reaction 
{
	work_order_reaction() = default;
	work_order_reaction(std::size_t id, Coordinates workshopCo, std::string reactionTag, std::vector<std::pair<std::size_t, bool>> components) 
		: workshop_id(id), workshopCo(workshopCo), reactionTag(reactionTag), components(components) {}

	std::size_t workshop_id;

	Coordinates workshopCo;

	std::string reactionTag;

	std::vector<std::pair<std::size_t, bool>> components;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(workshop_id, workshopCo, reactionTag, components);
	}
};
