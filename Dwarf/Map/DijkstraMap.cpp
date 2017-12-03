#include "DijkstraMap.h"
#include "Tile.h"

#include <deque>

DijkstraMap::DijkstraMap()
{
	distanceMap.resize(TOTAL_MAP_TILES);
	std::fill(distanceMap.begin(), distanceMap.end(), MAX_DIJKSTRA_DISTANCE);
}

void DijkstraMap::update(const std::vector<int>& startingPoints)
{
	std::thread{ &DijkstraMap::updateAsync, this, startingPoints }.detach();
}

Coordinates DijkstraMap::findDestination(const Coordinates co)
{
	return Coordinates();
}

int16_t DijkstraMap::get(const std::size_t & idx)
{
	std::lock_guard<std::mutex> lock(mapLock);
	return distanceMap[idx];
}

inline void dijkstra_add_candidate(std::deque<std::pair<int, int>> & openNodes, const Coordinates co, const int distance)
{
	const int idx = getIdx(co);

	openNodes.emplace_back(std::make_pair(idx, distance));
	// This will be usefull once we have other things to check
	// aside from just can go dir
}

void DijkstraMap::updateAsync(const std::vector<int>& startingPoints)
{
	std::vector<int16_t> newMap;
	newMap.resize(TOTAL_MAP_TILES);
	std::fill(newMap.begin(), newMap.end(), MAX_DIJKSTRA_DISTANCE);

	// Fill the open deque with starting points
	std::deque<std::pair<int, int>> openNodes;
	for (const int & startP : startingPoints)
		openNodes.emplace_back(std::make_pair(startP, 0));


	while (!openNodes.empty())
	{
		const std::pair<int, int> openNode = openNodes.front();
		openNodes.pop_front();

		if (newMap[openNode.first] > openNode.second && openNode.second < MAX_DIJKSTRA_DISTANCE)
		{
			newMap[openNode.first] = openNode.second;

			Coordinates co = idxToCo(openNode.first);

			if (co.y > 0 && tileManager->canGo<CAN_GO_NORTH>(co))
			{
				dijkstra_add_candidate(openNodes, { co.x, co.y - 1, co.z }, openNode.second + 1);
			}

			if (co.y < MAP_HEIGHT - 1 && tileManager->canGo<CAN_GO_SOUTH>(co))
			{
				dijkstra_add_candidate(openNodes, { co.x, co.y + 1, co.z }, openNode.second + 1);
			}

			if (co.x > 0 && tileManager->canGo<CAN_GO_EAST>(co))
			{
				dijkstra_add_candidate(openNodes, { co.x - 1, co.y, co.z }, openNode.second + 1);
			}

			if (co.y < MAP_HEIGHT - 1 && tileManager->canGo<CAN_GO_WEST>(co))
			{
				dijkstra_add_candidate(openNodes, { co.x, co.y - 1, co.z }, openNode.second + 1);
			}

			if (co.y > 0 && co.x > 0 && tileManager->canGo<CAN_GO_NORTH_W>(co))
			{
				dijkstra_add_candidate(openNodes, { co.x - 1, co.y - 1, co.z }, openNode.second + 1);
			}

			if (co.y > 0 && co.x < MAP_WIDTH - 1 && tileManager->canGo<CAN_GO_NORTH_E>(co))
			{
				dijkstra_add_candidate(openNodes, { co.x + 1, co.y - 1, co.z }, openNode.second + 1);
			}

			if (co.y < MAP_HEIGHT - 1 && co.x > 0 && tileManager->canGo<CAN_GO_SOUTH_W>(co))
			{
				dijkstra_add_candidate(openNodes, { co.x - 1, co.y + 1, co.z }, openNode.second + 1);
			}

			if (co.y < MAP_HEIGHT - 1 && co.x < MAP_WIDTH - 1 && tileManager->canGo<CAN_GO_SOUTH_E>(co))
			{
				dijkstra_add_candidate(openNodes, { co.x + 1, co.y - 1, co.z }, openNode.second + 1);
			}

			if (co.z < MAP_DEPTH - 1 && tileManager->canGo<CAN_GO_UP>(co))
			{
				dijkstra_add_candidate(openNodes, { co.x, co.y, co.z + 1}, openNode.second + 1);
			}

			if (co.z > 0 && tileManager->canGo<CAN_GO_DOWN>(co))
			{
				dijkstra_add_candidate(openNodes, { co.x, co.y, co.z - 1 }, openNode.second + 1);
			}
		}
	}

}


DijkstraMap::~DijkstraMap()
{
}
