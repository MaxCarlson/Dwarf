#pragma once
#include "../../Systems.h"
#include "../../Components/ItemStored.h"
#include <unordered_map>
#include <unordered_set>

struct StockpileInfo
{
	StockpileInfo() = default;
	StockpileInfo(std::size_t id, std::bitset<64> & catagory) : id(id), catagory(catagory) {}

	std::size_t id;
	std::bitset<64> catagory;
	int free_tiles;
	std::unordered_set<int> openTiles;
};

std::unordered_map<std::size_t, StockpileInfo> stockpiles;

class StockpileSystem : public System<Requires<>>
{
public:
	StockpileSystem() = default;

	void update();
	
};

