#pragma once
#include "../Systems.h"

#include <vector>
#include <unordered_map>

class LaborStatsComponent;
class JobComponent;
class TileManager;
struct Coordinates;

extern std::vector<uint8_t> miningMap;
extern std::vector<int> miningTargets;

class MiningSystem : public System<Requires<LaborStatsComponent, JobComponent>>
{
public:
	MiningSystem() = default;
	//~MiningSystem();

	void init(TileManager* tileManager_i);

	//void designateMining(Coordinates topLeft, Coordinates botRight); // Make a designations class

	void makeMiningMap();
	void walkMiningMap(const Coordinates co, const int distance, const int idx);
	
	//void findPick(); // Find a pickaxe before Entity starts mining (eventually)

private:
	TileManager * tileManager;
	int width, height, depth;
	void issueJob(); // Possibly make this to Emit a message to Job's system? // Possibly make an Entity have a queue of jobs that it needs to complete for easy mining? Or a vector of queues for each job type?
};