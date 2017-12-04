#pragma once

#include "../../Systems.h"
#include "../../../Map/DijkstraMap.h"

extern DijkstraMap pick_map;

class PickMapSystem;

class DijkstraMapsHandler : public System<Requires<>> // Does not interate through Entities, only handles other dijkstra map systems
{
public:
	DijkstraMapsHandler() = default;

	void init();
	void update();

private:
	// Dijk map system pointers
	std::unique_ptr<PickMapSystem> pickMapSystem;

	// bool's for updating
	bool update_pick_map = true;
};

