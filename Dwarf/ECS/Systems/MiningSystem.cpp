#include "MiningSystem.h"
#include "../Tile.h"
#include "../Designations.h"

std::vector<uint8_t> miningMap;
std::vector<int> miningTargets;

MiningSystem::MiningSystem()
{
}

void MiningSystem::init(TileManager* tileManager_i)
{
	tileManager = tileManager_i; 
	width = tileManager->width;
	height = tileManager->height;
	depth = tileManager->depth;
	miningMap.resize(width * depth * height);
	miningTargets.resize(width * depth * height);
}

void MiningSystem::makeMiningMap()
{
	std::fill(miningMap.begin(), miningMap.end(), std::numeric_limits<uint8_t>::max());
	std::fill(miningTargets.begin(), miningTargets.end(), std::numeric_limits<int>::max());

	std::vector<std::tuple<int, int, int, int>> startingPoints;

	for(int z = 0; z < depth; ++z)
		for(int y = 0; y < height; ++y)
			for (int x = 0; x < width; ++x)
			{
				Coordinates co = { x, y, z };
				const auto idx = TILE_ARRAY_LOOKUP;
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

	for (auto& pos : startingPoints)
		walkMiningMap({ std::get<0>(pos), std::get<1>(pos), std::get<2>(pos) }, 0, std::get<3>(pos));
}

void MiningSystem::walkMiningMap(const Coordinates co, const int distance, const int idx)
{
	if (distance > 200)
		return;

	const auto newIdx = TILE_ARRAY_LOOKUP;

	if (miningMap[newIdx] > distance)
	{
		if (!tileManager->canWalk(co))
			return;

		miningMap[newIdx] = distance;
		miningTargets[newIdx] = idx;

		if (tileManager->canGo<CAN_GO_NORTH>(co)) walkMiningMap({ co.x, co.y - 1, co.z }, distance + 1, idx);
		if (tileManager->canGo<CAN_GO_SOUTH>(co)) walkMiningMap({ co.x, co.y + 1, co.z }, distance + 1, idx);
		if (tileManager->canGo<CAN_GO_EAST >(co)) walkMiningMap({ co.x + 1, co.y, co.z }, distance + 1, idx);
		if (tileManager->canGo<CAN_GO_WEST >(co)) walkMiningMap({ co.x - 1, co.y, co.z }, distance + 1, idx);
		if (tileManager->canGo<CAN_GO_UP   >(co)) walkMiningMap({ co.x, co.y, co.z + 1 }, distance + 1, idx);
		if (tileManager->canGo<CAN_GO_DOWN >(co)) walkMiningMap({ co.x, co.y, co.z - 1 }, distance + 1, idx);
	}
}

void MiningSystem::issueJob()
{
}


