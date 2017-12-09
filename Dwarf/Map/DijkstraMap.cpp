#include "DijkstraMap.h"
#include "Tile.h"

#include <deque>
#include <map>

using namespace region;

void DijkstraMap::init()
{
	distanceMap.resize(TOTAL_MAP_TILES);
	std::fill(distanceMap.begin(), distanceMap.end(), MAX_DIJKSTRA_DISTANCE);
}

void DijkstraMap::update(const std::vector<int>& startingPoints)
{
	// Make a thread pool!!!
	std::thread{ &DijkstraMap::updateAsync, this, startingPoints }.detach();
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

			if (flag(co, CAN_GO_NORTH))
			{
				dijkstra_add_candidate(openNodes, CO_NORTH, openNode.second + 1);
			}

			if (flag(co, CAN_GO_SOUTH))
			{
				dijkstra_add_candidate(openNodes, CO_SOUTH, openNode.second + 1);
			}

			if (flag(co, CAN_GO_EAST))
			{
				dijkstra_add_candidate(openNodes, CO_EAST, openNode.second + 1);
			}

			if (flag(co, CAN_GO_WEST))
			{
				dijkstra_add_candidate(openNodes, CO_WEST, openNode.second + 1);
			}

			if (flag(co, CAN_GO_NORTH_W))
			{
				dijkstra_add_candidate(openNodes, CO_NORTH_W, openNode.second + 1);
			}

			if (flag(co, CAN_GO_NORTH_E))
			{
				dijkstra_add_candidate(openNodes, CO_NORTH_E, openNode.second + 1);
			}

			if (flag(co, CAN_GO_SOUTH_W))
			{
				dijkstra_add_candidate(openNodes, CO_SOUTH_W, openNode.second + 1);
			}

			if (flag(co, CAN_GO_SOUTH_E))
			{
				dijkstra_add_candidate(openNodes, CO_SOUTH_E, openNode.second + 1);
			}

			if (flag(co, CAN_GO_UP))
			{
				dijkstra_add_candidate(openNodes, CO_UP, openNode.second + 1);
			}

			if (flag(co, CAN_GO_DOWN))
			{
				dijkstra_add_candidate(openNodes, CO_DOWN, openNode.second + 1);
			}
		}
	}

	std::lock_guard<std::mutex> lock(mapLock);
	distanceMap = newMap;
}

Coordinates DijkstraMap::findDestination(const Coordinates co)
{
	std::lock_guard<std::mutex> lock(mapLock);

	const int idx = getIdx(co);
	std::map<int32_t, int> candidnates;

	if (flag(co, CAN_GO_NORTH))
	{
		const int dest = getIdx(CO_NORTH);
		candidnates.insert(std::make_pair(distanceMap[dest], dest));
	}

	if (flag(co, CAN_GO_SOUTH))
	{
		const int dest = getIdx(CO_SOUTH);
		candidnates.insert(std::make_pair(distanceMap[dest], dest));
	}

	if (flag(co, CAN_GO_EAST))
	{
		const int dest = getIdx(CO_EAST);
		candidnates.insert(std::make_pair(distanceMap[dest], dest));
	}

	if (flag(co, CAN_GO_WEST))
	{
		const int dest = getIdx(CO_WEST);
		candidnates.insert(std::make_pair(distanceMap[dest], dest));
	}

	if (flag(co, CAN_GO_NORTH_W))
	{
		const int dest = getIdx(CO_NORTH_W);
		candidnates.insert(std::make_pair(distanceMap[dest], dest));
	}

	if (flag(co, CAN_GO_NORTH_E))
	{
		const int dest = getIdx(CO_NORTH_E);
		candidnates.insert(std::make_pair(distanceMap[dest], dest));
	}

	if (flag(co, CAN_GO_SOUTH_W))
	{
		const int dest = getIdx(CO_SOUTH_W);
		candidnates.insert(std::make_pair(distanceMap[dest], dest));
	}

	if (flag(co, CAN_GO_SOUTH_E))
	{
		const int dest = getIdx(CO_SOUTH_E);
		candidnates.insert(std::make_pair(distanceMap[dest], dest));
	}

	if (flag(co, CAN_GO_UP))
	{
		const int dest = getIdx(CO_UP);
		candidnates.insert(std::make_pair(distanceMap[dest], dest));
	}

	if (flag(co, CAN_GO_DOWN))
	{
		const int dest = getIdx(CO_DOWN);
		candidnates.insert(std::make_pair(distanceMap[dest], dest));
	}

	return idxToCo(candidnates.begin()->second);
}