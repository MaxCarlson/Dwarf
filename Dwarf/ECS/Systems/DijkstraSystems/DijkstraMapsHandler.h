#pragma once

#include "../../Systems.h"
#include "../../../Map/DijkstraMap.h"

extern DijkstraMap pick_map;

class PickMapSystem;

class DijkstraMapsHandler : public System<Requires<>, Excludes<>, Any<>> // Update any and get rid of other lower dijkstra systems?
{
public:
	DijkstraMapsHandler() = default;
	~DijkstraMapsHandler();

	void init();
	void update();

private:
	// Dijk map system pointers
	PickMapSystem * pickMapSystem;

	// bool's for updating
	bool update_pick_map = true;
};

