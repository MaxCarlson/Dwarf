#pragma once
#include "../../Systems.h"

class LaborStatsComponent;
class JobComponent;
class MiningSystem;
class TileFactory;

class MiningAiSystem : public System<Requires<LaborStatsComponent, JobComponent >>
{
public:
	MiningAiSystem() = default;
	MiningAiSystem(MiningSystem * miningSystem, TileFactory * tileFactory);

	void update();

	int currentJobs;

private:
	MiningSystem * miningSystem;

	// Should probably be using the TileCreator class so we can
	// change a tile to a floor tile with just a function
	TileFactory * tileFactory;

	void mineTile(const Entity e);
};

