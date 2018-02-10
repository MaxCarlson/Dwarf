#include "../stdafx.h"
#include "StockpileSystem.h"
#include "../helpers/ItemHelper.h"
#include "../../../Map/Tile.h"
#include "../../Components/Claimed.h"
#include "../../Components/RenderComponent.h"
#include "../../../Raws/ItemRead.h"
#include "../../../Raws/Defs/ItemDefs.h"
#include "../Designations.h"

std::unordered_map<std::size_t, StockpileInfo> stockpiles;

// Map of Stockpiles id's that match stockpile type int
std::unordered_map<int, std::vector<std::size_t>> stockpileTargets;

std::vector<StoreableItem> storeableItems;

void StockpileSystem::init()
{
}

void StockpileSystem::update(const double duration)
{
	stockpiles.clear();
	stockpileTargets.clear();
	storeableItems.clear();

	bool stockpilesExist = false;

	for (const auto& e : getEntities())
	{
		stockpilesExist = true;

		auto& stock = e.getComponent<Stockpile>();
		const auto sid = static_cast<size_t>(e.getId().index);

		StockpileInfo info = { sid, stock.catagory };
		stockpiles[sid] = info;

		for(int i = 0; i < stock.catagory.size(); ++i)
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

	// For all items currently being hauled, or about to be hauled
	// be sure to remove the squares they're going to be put on
	// from the list of open squares in each stockpile
	for (const auto& h : designations->hauling)
	{
		--stockpiles[h.second].freeSpots;
		stockpiles[h.second].openTiles.erase(h.first);
	}

	// Reduce the free tiles of stockpiles
	// with items on their squares
	itemHelper.forEachItem([](auto& e) // Looping through all items with position components seems inefficiant, If perf issue - Loop through stockpile squares and find items with position cahce lookups
		{
			if (!e.hasComponent<PositionComponent>())
				return;

			const int idx = getIdx(e.getComponent<PositionComponent>().co);

			// If there's an item on the stockpile square 
			if (region::stockpileId(idx) > 0)
			{
				--stockpiles[region::stockpileId(idx)].freeSpots;
				stockpiles[region::stockpileId(idx)].openTiles.erase(idx);
				return;
			}
		}
	);

	itemHelper.forEachItem([](auto& e) // Looping through all items with position components seems inefficiant, If perf issue - Loop through stockpile squares and find items with position cahce lookups
	{
		// Skip claimed items
		if (e.hasComponent<Claimed>())
			return;

		// Skip already stockpiled items ~~ This will cause items in wrong stockpiles to not be moved to the correct ones!
		const int idx = getIdx(e.getComponent<PositionComponent>().co);
		if (region::stockpileId(idx) > 0)
			return;

		const auto stock_id = getItemDef(e.getComponent<Item>().tag)->stockpileId;

		// Item doesn't have stockpile type
		if (!stock_id)
			return;

		// Find all stockpiles that are accepting this item type
		const auto& find = stockpileTargets.find(stock_id);

		// No stockpiles that can take this item
		if (find == stockpileTargets.end())
			return;

		// Find a stockpile with storage space
		// and add item id and free tile location to list of storeable items and their destinations
		for (const auto& id : find->second)
		{
			if (stockpiles[id].freeSpots > 0)
			{
				--stockpiles[id].freeSpots;
				int destination = *stockpiles[id].openTiles.begin();

				stockpiles[id].openTiles.erase(destination);
				storeableItems.push_back(StoreableItem{ e.getId().index, destination });
				return;
			}
		}
	});
}

