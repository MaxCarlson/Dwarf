#pragma once

#include "../Coordinates.h"
#include <vector>
#include <mutex>

constexpr int16_t MAX_DIJKSTRA_DISTANCE = 256;

class DijkstraMap
{
public:
	DijkstraMap() = default;
	void init();

	void update(const std::vector<int>& startingPoints);

	Coordinates findDestination(const Coordinates co);

	int16_t get(const std::size_t &idx);

private:
	std::mutex mapLock;

	std::vector<int16_t> distanceMap;

	void updateAsync(const std::vector<int>& startingPoints);
};

