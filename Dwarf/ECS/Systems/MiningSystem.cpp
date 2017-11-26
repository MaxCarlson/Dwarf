#include "MiningSystem.h"
#include "../Tile.h"
#include "../Designations.h"

std::vector<uint8_t> miningMap;
std::vector<int> miningTargets;

MiningSystem::MiningSystem()
{
}

void MiningSystem::init(int width_i, int height_i, int depth_i)
{
	width = width_i;
	height = height_i;
	depth = depth_i;
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
}

void MiningSystem::issueJob()
{
}


