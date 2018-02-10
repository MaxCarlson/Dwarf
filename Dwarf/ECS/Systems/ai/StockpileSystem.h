#pragma once
#include "../../Systems.h"
#include "../../Components/Stockpile.h"
#include <unordered_map>
#include <unordered_set>

struct StockpileInfo
{
	StockpileInfo() = default;
	StockpileInfo(std::size_t id, std::bitset<64> & catagory) : id(id), catagory(catagory) {}

	std::size_t id;
	std::bitset<64> catagory;
	int freeSpots = 0;
	std::unordered_set<int> openTiles;
};

struct StoreableItem
{
	StoreableItem() = default;
	StoreableItem(std::size_t itemId, int desination) : itemId(itemId), destination(desination) {}

	std::size_t itemId;
	int destination;
};

extern std::unordered_map<std::size_t, StockpileInfo> stockpiles;
extern std::unordered_map<int, std::vector<std::size_t>> stockpileTargets;
extern std::vector<StoreableItem> storeableItems;

class StockpileSystem : public System<Requires<Stockpile>>
{
public:
	StockpileSystem() = default;

	void init();

	void update(const double duration);

private:
};

