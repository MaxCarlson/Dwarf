#pragma once

#include "../../Systems.h"
#include "../../../Map/DijkstraMap.h"

extern DijkstraMap pick_map;
extern DijkstraMap block_map;
extern DijkstraMap architecture_map;
extern DijkstraMap axe_map;
extern DijkstraMap harvest_map;
extern DijkstraMap planting_map;
extern DijkstraMap hoe_map;

class PickMapSystem;

class DijkstraMapsHandler : public System<Requires<>> 
{
public:
	DijkstraMapsHandler() = default;
	~DijkstraMapsHandler();

	void init();
	void update(const double duration);

private:

	// bool's for updating
	bool update_all_maps = true;
	bool update_pick_map = true;
	bool update_axe_map = true;
	bool update_block_map = true;
	bool update_architecture = true;
	bool update_harvest = true;
	bool update_planting = true;
	bool update_hoe = true;
};

