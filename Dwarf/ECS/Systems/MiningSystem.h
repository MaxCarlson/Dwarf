#pragma once
#include "../Systems.h"

#include <vector>
#include <unordered_map>

class LaborStatsComponent;
class JobComponent;
class TileManager;
struct Coordinates;
class JobsSystem;

//extern std::vector<uint8_t> miningMap;
//extern std::vector<int> miningTargets;

class MiningSystem : public System<Requires<>>//Requires<LaborStatsComponent, JobComponent>> // Does not interate through Entities
{
public:
	MiningSystem() = default;
	MiningSystem(TileManager* tileManager, JobsSystem * jobsSystem);

	void update();

	void findStartingPoints();
	inline void regularDigging(const Coordinates co);

	//void makeMiningMap();
	//void walkMiningMap(const Coordinates co, const int distance, const int idx);	
	//void findPick(); // Find a pickaxe before Entity starts mining (eventually)

private:
	std::vector<std::pair<Coordinates, Coordinates>> startingPoints; // Possibly make a vector for each of the different types of mining jobs?

	TileManager * tileManager;
	JobsSystem  *  jobsSystem;

	int width, height, depth;
	void issueJobs(); // Possibly make this to Emit a message to Job's system? // Possibly make an Entity have a queue of jobs that it needs to complete for easy mining? Or a vector of queues for each job type?
};