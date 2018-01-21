#include "MiningSystem.h"
#include "../Map/Tile.h"
#include "../Designations.h"
#include "../Messages/recalculate_mining_message.h"
#include "../Messages/perform_mining_message.h"
#include "../World.h"
#include "../Components/LaborStatsComponent.h"
#include "../Raws/raws.h"
#include "../Raws/Materials.h"
#include "../Raws/Defs/MaterialDef.h"

using namespace region;

// Map of distances to designated mining
// targets
std::vector<uint8_t> miningMap;

// The location of the tile being mined
// Indexed by the idx of the current Entities positon
std::vector<int> miningTargets;

void MiningSystem::init()
{
	subscribe<recalculate_mining_message>([this](recalculate_mining_message &msg)
	{
		mapDirty = true;
	});

	miningMap.resize(MAP_WIDTH * MAP_HEIGHT * MAP_DEPTH);
	miningTargets.resize(MAP_WIDTH * MAP_HEIGHT * MAP_DEPTH);
	makeMiningMap();
}

void MiningSystem::update()
{
	if (mapDirty)
	{
		makeMiningMap();
		mapDirty = false;
	}
}

void MiningSystem::makeMiningMap()
{
	std::fill(miningMap.begin(), miningMap.end(), std::numeric_limits<uint8_t>::max());
	std::fill(miningTargets.begin(), miningTargets.end(), std::numeric_limits<int>::max());

	std::vector<std::tuple<int, int, int, int>> startingPoints;

	for(int z = 1; z < MAP_DEPTH - 2; ++z)
		for(int y = 1; y < MAP_HEIGHT - 2; ++y)
			for (int x = 1; x < MAP_WIDTH - 2; ++x)
			{
				const auto idx = getIdx({ x, y, z });
				auto des = designations->mining.find(idx);

				if (des != designations->mining.end())
				{
					switch (des->second)
					{
					case 0: // Regular digging
					startingPoints.push_back(std::make_tuple(x - 1, y, z, idx));
					startingPoints.push_back(std::make_tuple(x + 1, y, z, idx));
					startingPoints.push_back(std::make_tuple(x, y - 1, z, idx));
					startingPoints.push_back(std::make_tuple(x, y + 1, z, idx));
					break;

					// Everything after here will not be implemented for a while
					case 1: // Channeling
					startingPoints.push_back(std::make_tuple(x, y, z, idx));
					startingPoints.push_back(std::make_tuple(x, y, z - 1, idx));
					startingPoints.push_back(std::make_tuple(x - 1, y, z, idx));
					startingPoints.push_back(std::make_tuple(x + 1, y, z, idx));
					startingPoints.push_back(std::make_tuple(x, y - 1, z, idx));
					startingPoints.push_back(std::make_tuple(x, y + 1, z, idx));
					break;

					case 2: // Ramping
					startingPoints.push_back(std::make_tuple(x, y, z, idx));
					startingPoints.push_back(std::make_tuple(x, y, z + 1, idx));
					startingPoints.push_back(std::make_tuple(x - 1, y, z - 1, idx));
					startingPoints.push_back(std::make_tuple(x + 1, y, z - 1, idx));
					startingPoints.push_back(std::make_tuple(x, y - 1, z - 1, idx));
					startingPoints.push_back(std::make_tuple(x, y + 1, z - 1, idx));
					break;

					case 3: // Up stairs
					startingPoints.push_back(std::make_tuple(x, y, z, idx));
					startingPoints.push_back(std::make_tuple(x, y, z + 1, idx));
					startingPoints.push_back(std::make_tuple(x - 1, y, z, idx));
					startingPoints.push_back(std::make_tuple(x + 1, y, z, idx));
					startingPoints.push_back(std::make_tuple(x, y - 1, z, idx));
					startingPoints.push_back(std::make_tuple(x, y + 1, z, idx));
					break;

					case 4: // Down stairs
					startingPoints.push_back(std::make_tuple(x, y, z, idx));
					startingPoints.push_back(std::make_tuple(x, y, z - 1, idx));
					startingPoints.push_back(std::make_tuple(x - 1, y, z, idx));
					startingPoints.push_back(std::make_tuple(x + 1, y, z, idx));
					startingPoints.push_back(std::make_tuple(x, y - 1, z, idx));
					startingPoints.push_back(std::make_tuple(x, y + 1, z, idx));
					break;

					case 5: // Up down stairs
					startingPoints.push_back(std::make_tuple(x, y, z, idx));
					startingPoints.push_back(std::make_tuple(x, y, z - 1, idx));
					startingPoints.push_back(std::make_tuple(x, y, z + 1, idx));
					startingPoints.push_back(std::make_tuple(x - 1, y, z, idx));
					startingPoints.push_back(std::make_tuple(x + 1, y, z, idx));
					startingPoints.push_back(std::make_tuple(x, y - 1, z, idx));
					startingPoints.push_back(std::make_tuple(x, y + 1, z, idx));
					break;
					}
				}
			}

	// Loop through starting points and generate
	// a mining distance map
	for (auto& pos : startingPoints)
		walkMiningMap({ std::get<0>(pos), std::get<1>(pos), std::get<2>(pos) }, 0, std::get<3>(pos));
}

void MiningSystem::walkMiningMap(const Coordinates co, const int distance, const int idx)
{
	// This is too small!!?
	if (distance > 250)
		return;

	const int newIdx = getIdx(co);

	if (miningMap[newIdx] > distance)
	{
		if (!flag(co, CAN_STAND_HERE))
			return;

		miningMap[newIdx] = distance;
		miningTargets[newIdx] = idx;

		// Probably multi-threaded calcuating needs to be done
		// when updating these map flags
		// Medium-High priority		
		if (flag(co, CAN_GO_NORTH)) walkMiningMap({ co.x, co.y - 1, co.z }, distance + 1, idx);
		if (flag(co, CAN_GO_SOUTH)) walkMiningMap({ co.x, co.y + 1, co.z }, distance + 1, idx);
		if (flag(co,  CAN_GO_EAST)) walkMiningMap({ co.x + 1, co.y, co.z }, distance + 1, idx);
		if (flag(co,  CAN_GO_WEST)) walkMiningMap({ co.x - 1, co.y, co.z }, distance + 1, idx);
		if (flag(co,    CAN_GO_UP)) walkMiningMap({ co.x, co.y, co.z + 1 }, distance + 1, idx);
		if (flag(co,  CAN_GO_DOWN)) walkMiningMap({ co.x, co.y, co.z - 1 }, distance + 1, idx);
	}
}
