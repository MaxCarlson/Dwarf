#pragma once

#include "../../Systems.h"
#include "../../../Map/DijkstraMap.h"

extern DijkstraMap pick_map;
extern DijkstraMap block_map;
extern DijkstraMap architecture_map;

class PickMapSystem;

class DijkstraMapsHandler : public System<Requires<>, Excludes<>, Any<>> 
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
	bool update_all_maps = true;
	bool update_pick_map = true;
	bool update_block_map = true;
	bool update_architecture = true;
};

