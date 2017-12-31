#include "StockpileSystem.h"
#include "../helpers/ItemHelper.h"
#include "../../../Map/Tile.h"
#include "../../Components/Claimed.h"
#include "../../../Raws/ItemRead.h"
#include "../../../Raws/Defs/ItemDefs.h"

std::unordered_map<std::size_t, StockpileInfo> stockpiles;

// Map of Stockpiles of type int
std::unordered_map<int, std::vector<std::size_t>> stockpileTargets;


std::vector<StoreableItem> storeableItems;

void StockpileSystem::update()
{
	stockpiles.clear();
	stockpileTargets.clear();
	storeableItems.clear();

	bool stockpilesExist = false;

	for (auto& e : getEntities())
	{
		stockpilesExist = true;

		auto& stock = e.getComponent<Stockpile>();
		const auto  sid = e.getId().index;

		StockpileInfo info = { sid, stock.catagory };
		stockpiles[sid] = info;

		for(int i = 0; i <= stock.catagory.size(); ++i)
			if (stock.catagory.test(i))
			{
				auto f = stockpileTargets.find(sid);

				if (f == stockpileTargets.end())
					stockpileTargets[i] = std::vector<std::size_t>{ sid };

				else
					stockpileTargets[i].push_back(sid);
			}

	}

	if (!stockpilesExist)
		return;


	// Build (and count) list of free tiles for each stockpile
	region::forStockpileSquares([](const auto idx, const auto id)
		{
			if (id > 0)
			{
				++stockpiles[id].freeSpots;
				stockpiles[id].openTiles.insert(idx);
			}
		}
	);

	itemHelper.forEachItem([](auto& e) // Looping through all items with position components seems inefficiant, If perf issue - Loop through stockpile squares and find items with position cahce lookups
		{
		const int idx = getIdx(e.getComponent<PositionComponent>().co);

		// If there's an item on the stockpile square
		if (region::stockpileId(idx) > 0)
		{
			--stockpiles[region::stockpileId(idx)].freeSpots;
			stockpiles[region::stockpileId(idx)].openTiles.erase(idx);
		}

		// Skip claimed items
		if (e.hasComponent<Claimed>())
			return;


		auto stock_id = getItemDef(e.getComponent<Item>().tag)->stockpileId;

		// Item doesn't have stockpile type
		if (!stock_id)
			return;

		const auto& find = stockpileTargets.find(stock_id);

		// No stockpiles that can take this item
		if (find == stockpileTargets.end())
			return;

		// Find a stockpile with storage space
		// and add item id to list of storeable items
		for (auto id : find->second)
		{
			if (stockpiles[id].freeSpots > 0)
			{
				--stockpiles[id].freeSpots;
				int desination = *stockpiles[id].openTiles.begin();

				stockpiles[id].openTiles.erase(desination);
				storeableItems.push_back(StoreableItem{ e.getId().index, desination });
				return;
			}
		}

		}
	);
}
