#include "DefInfo.h"
#include "../Raws\ReadReactions.h"
#include "../Raws\ItemRead.h"
#include "../Raws\Materials.h"
#include "../Raws\Defs\MaterialDef.h"
#include "../Raws\Buildings.h"
#include "../Designations.h"

std::unique_ptr<DefInfo> defInfo;

void DefInfo::init()
{
	buildingTags = get_building_tags();
	buildingNames = get_all_building_def_names();
	itemTags = get_all_item_tags();

	for(const auto& b : builtBuildings)
		updateBuildingReactions(b, true);
}

void DefInfo::updateBuildingReactions(const std::string buildingTag, bool first)
{
	if (first || std::find(builtBuildings.begin(), builtBuildings.end(), buildingTag) != builtBuildings.end()) // Possibly make availibleBuildings an unordered_map if there is a speed issue here
		return;


	builtBuildings.push_back(buildingTag);

	availibleReactions[buildingTag] = getReactionsByWorkshop(buildingTag);
}
