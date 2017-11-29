#pragma once
#include "../Systems.h"

#include <vector>
#include <unordered_map>

class LaborStatsComponent;
class JobComponent;
class TileManager;
struct Coordinates;
class JobsSystem;
class Job;

extern std::vector<uint8_t> miningMap;
extern std::vector<int> miningTargets;

class MiningSystem : public System<Requires<>> // Does not interate through Entities
{
public:
	MiningSystem() = default;
	MiningSystem(TileManager* tileManager);

	void update();

	void makeMiningMap();
	void walkMiningMap(const Coordinates co, const int distance, const int idx);	

private:
	int width, height, depth;
	TileManager * tileManager;
};