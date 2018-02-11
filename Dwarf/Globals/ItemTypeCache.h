#pragma once
#include <cereal\archives\binary.hpp>

struct ItemTypeCache
{

	std::unordered_map<std::string, int> seedTypes;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(seedTypes);
	}
};

extern ItemTypeCache itemCache;