#include "MiningSystem.h"
#include "../Map/Tile.h"
#include "../Designations.h"
#include "../Messages/recalculate_mining_message.h"
#include "../Messages/perform_mining_message.h"
#include "../World.h"
#include "../TileFactory.h"
#include "../Components/LaborStatsComponent.h"

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
		makeMiningMap();
	});

	subscribe<perform_mining_message>([this](perform_mining_message &msg)
	{
		performMining(msg.e, msg.targetIdx, msg.opperation);
	});

	miningMap.resize(MAP_WIDTH * MAP_HEIGHT * MAP_DEPTH);
	miningTargets.resize(MAP_WIDTH * MAP_HEIGHT * MAP_DEPTH);
}

void MiningSystem::update()
{
	return; 
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
					case 1: // Regular digging
					startingPoints.push_back(std::make_tuple(x - 1, y, z, idx));
					startingPoints.push_back(std::make_tuple(x + 1, y, z, idx));
					startingPoints.push_back(std::make_tuple(x, y - 1, z, idx));
					startingPoints.push_back(std::make_tuple(x, y + 1, z, idx));
					break;

					// Everything after here will not be implemented for a while
					case 2: // Channeling
					startingPoints.push_back(std::make_tuple(x, y, z, idx));
					startingPoints.push_back(std::make_tuple(x, y, z - 1, idx));
					startingPoints.push_back(std::make_tuple(x - 1, y, z, idx));
					startingPoints.push_back(std::make_tuple(x + 1, y, z, idx));
					startingPoints.push_back(std::make_tuple(x, y - 1, z, idx));
					startingPoints.push_back(std::make_tuple(x, y + 1, z, idx));
					break;

					case 3: // Ramping
					startingPoints.push_back(std::make_tuple(x, y, z, idx));
					startingPoints.push_back(std::make_tuple(x, y, z + 1, idx));
					startingPoints.push_back(std::make_tuple(x - 1, y, z - 1, idx));
					startingPoints.push_back(std::make_tuple(x + 1, y, z - 1, idx));
					startingPoints.push_back(std::make_tuple(x, y - 1, z - 1, idx));
					startingPoints.push_back(std::make_tuple(x, y + 1, z - 1, idx));
					break;

					case 4: // Up stairs
					startingPoints.push_back(std::make_tuple(x, y, z, idx));
					startingPoints.push_back(std::make_tuple(x, y, z + 1, idx));
					startingPoints.push_back(std::make_tuple(x - 1, y, z, idx));
					startingPoints.push_back(std::make_tuple(x + 1, y, z, idx));
					startingPoints.push_back(std::make_tuple(x, y - 1, z, idx));
					startingPoints.push_back(std::make_tuple(x, y + 1, z, idx));
					break;

					case 5: // Down stairs
					startingPoints.push_back(std::make_tuple(x, y, z, idx));
					startingPoints.push_back(std::make_tuple(x, y, z - 1, idx));
					startingPoints.push_back(std::make_tuple(x - 1, y, z, idx));
					startingPoints.push_back(std::make_tuple(x + 1, y, z, idx));
					startingPoints.push_back(std::make_tuple(x, y - 1, z, idx));
					startingPoints.push_back(std::make_tuple(x, y + 1, z, idx));
					break;

					case 6: // Up down stairs
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
	if (distance > 200)
		return;

	const auto newIdx = TILE_ARRAY_LOOKUP;

	if (miningMap[newIdx] > distance)
	{
		if (!flag(co, CAN_STAND_HERE))
			return;

		miningMap[newIdx] = distance;
		miningTargets[newIdx] = idx;

		// All of this needs to be pre-calcuated!!!!
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

void MiningSystem::performMining(Entity e, const int targetIdx, const uint8_t miningType)
{
	Tile& t = tileAt(targetIdx);

	if (t.health > 0)
	{

		auto& labor = e.getComponent<LaborStatsComponent>();

		// Health needs to be changed to be dependant on material
		t.health -= 4 * JobSpeedMultiplier[labor.laborLevel[int(Jobs::MINER)]];

		labor.skillPoints[int(Jobs::MINER)] += 2;

		return;
	}

	// Need to add in code to increase level when skill points
	// reach critical point, Possibly in just a sepperate system?

	// Remove the designation and change the tile
	designations->mining.erase(targetIdx);

	// Eventually this should probably be a template that takes
	// a tile material as a param and produces a floor of that material
	tileFactory->createRockFloor(idxToCo(targetIdx));


	tileRecalcAll(); // This should be a partial recalc based on tile mined!!

	makeMiningMap();
}