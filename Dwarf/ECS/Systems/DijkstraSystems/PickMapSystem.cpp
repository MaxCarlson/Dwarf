#include "PickMapSystem.h"
#include "../../../Raws/Defs/ItemDefs.h"
#include "../../../Map/Tile.h"
#include "DijkstraMapsHandler.h"

void PickMapSystem::init()
{

}

void PickMapSystem::update()
{
	std::vector<int> targets;

	for (auto& e : getEntities())
	{
		auto& item = e.getComponent<Item>();

		if (!item.catagory.test(TOOL_DIGGING) || item.claimed)
			return;

		// Claimed component ?

		auto& co = e.getComponent<PositionComponent>().co;

		targets.emplace_back(getIdx(co));

		// Need to add inventory handling?
	}

	pick_map.update(targets);
}

