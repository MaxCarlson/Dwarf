#pragma once
#include "cereal.hpp"
#include "../cereal/types/utility.hpp"

struct work_order_reaction 
{
	work_order_reaction() = default;
	work_order_reaction(std::size_t id, std::string reactionTag, std::vector<std::pair<std::size_t, bool>> components) : workshop_id(id), reactionTag(reactionTag), components(components) {}

	std::size_t workshop_id;

	std::string reactionTag;

	std::vector<std::pair<std::size_t, bool>> components;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(workshop_id);
		archive(reactionTag);
		archive(components);
	}
};
