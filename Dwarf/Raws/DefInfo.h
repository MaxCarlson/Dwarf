#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <cereal.hpp>
#include "../cereal/types/vector.hpp"

struct DefInfo
{
	DefInfo() = default;

	void init();

	// These are dependant on saves
	// Buildings that have been built
	std::vector<std::string> builtBuildings;

	// These change depending on defs
	std::vector<std::string> buildingTags;
	std::vector<std::string> buildingNames;

	std::vector<std::string> itemTags;
	std::vector<std::string> materialTags;

	std::vector<std::string> availibleReactions;
	std::unordered_map<std::string, std::vector<std::string>> avReactionsByBuilding;

	std::vector<std::string> stockpileTags;
	std::vector<std::string> stockpileNames;

	void updateBuildingReactions(const std::string buildingTag, bool first = false);

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(builtBuildings);
	}
};

extern std::unique_ptr<DefInfo> defInfo;

