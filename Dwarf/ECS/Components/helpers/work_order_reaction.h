#pragma once
#include "cereal.hpp"
#include "../cereal/types/utility.hpp"

struct work_order_reaction 
{
	work_order_reaction() = default;

	std::size_t workshop_id;

	//std::string jobName;

	std::vector<std::pair<std::string, std::size_t>> components;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(workshop_id);
		archive(components);
	}
};
