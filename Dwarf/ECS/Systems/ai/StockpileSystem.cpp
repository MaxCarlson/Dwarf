#include "../stdafx.h"
#include "StockpileSystem.h"
#include "../helpers/ItemHelper.h"
#include "../../../Map/Tile.h"
#include "../../Components/Claimed.h"
#include "../../Components/RenderComponent.h"
#include "../../../Raws/ItemRead.h"
#include "../../../Raws/Defs/ItemDefs.h"
#include "../../Messages/request_new_stockpile_message.h"
#include "../Designations.h"

std::unordered_map<std::size_t, StockpileInfo> stockpiles;

// Map of Stockpiles id's that match stockpile type int
std::unordered_map<int, std::vector<std::size_t>> stockpileTargets;

std::vector<StoreableItem> storeableItems;

void StockpileSystem::createNewStockpile(std::pair<int, int>& area, const std::bitset<64> &catagories)
{
	auto co1 = idxToCo(area.first);
	auto co2 = idxToCo(area.second);

	if (co1.z != co2.z)
		return;

	if (co1.x > co2.x)
	{
		int tmp = co1.x;
		co1.x = co2.x;
		co2.x = tmp;
	}
	if (co1.y > co2.y)
	{
		int tmp = co1.y;
		co1.y = co2.y;
		co2.y = tmp;
	}

	auto newstock = getWorld().createEntity();
	auto sid = newstock.getId().index;

	newstock.addComponent<Stockpile>(Stockpile{catagories});
	newstock.addComponent<PositionComponent>(); // Allows rendering
	newstock.addComponent<RenderComponent>();  // ^^

	for (int x = co1.x; x <= co2.x; ++x)
		for (int y = co1.y; y <= co2.y; ++y)
		{
			const Coordinates pos = { x, y, co1.z };
			const auto idx = getIdx(pos);

			// Don't let just any tile be designated a stockpile
			// Wrap around spurious designations
			// No overwriting old stockpiles ~~ atleast for the moment
			if (!region::flag(pos, region::Flag::CONSTRUCTION) && !region::solid(idx) && region::flag(pos, region::Flag::CAN_STAND_HERE) && region::stockpileId(idx) == 0)
			{
				region::setStockpileId(idx, sid);
			}			
		}

	newstock.activate();
}

void StockpileSystem::init()
{
	subscribe<request_new_stockpile_message>([this](request_new_stockpile_message &msg)
	{
		createNewStockpile(msg.area, msg.setCatagories);
	});
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
		const auto sid = e.getId().index;

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

		// Skip already stockpiled items
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

