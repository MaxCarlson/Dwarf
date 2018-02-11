#pragma once
#include <cereal\archives\binary.hpp>

struct ItemTypeCache
{
	// Seed types indexed by plant tag, vector contains
	// list of seed entity id's
	std::unordered_map<std::string, std::vector<size_t>> seedTypes;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(seedTypes);
	}
};

extern ItemTypeCache itemCache;