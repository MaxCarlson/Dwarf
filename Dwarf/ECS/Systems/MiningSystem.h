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

//extern std::vector<uint8_t> miningMap;
//extern std::vector<int> miningTargets;

class MiningSystem : public System<Requires<>>//Requires<LaborStatsComponent, JobComponent>> // Does not interate through Entities
{
public:
	MiningSystem() = default;
	MiningSystem(TileManager* tileManager, JobsSystem * jobsSystem);

	void update();

	void updateMap() { mapUpdate = true; }
	void updateCurrentJobs(Job job);

	void findStartingPoints();
	inline void regularDigging(const Coordinates co);

	//void makeMiningMap();
	//void walkMiningMap(const Coordinates co, const int distance, const int idx);	
	//void findPick(); // Find a pickaxe before Entity starts mining (eventually)

private:
	int width, height, depth;
	TileManager * tileManager;
	JobsSystem  *  jobsSystem;
	std::vector<std::pair<Coordinates, Coordinates>> startingPoints; // Possibly make a vector for each of the different types of mining jobs?

	// This is a vector of all active mining jobs
	// used so we don't give the same job to multiple
	// Entities
	std::vector<Job> currentJobs;

	// Set to true whenever the map changes
	// Really need to write a messaging system with boost bind
	// or signals
	bool mapUpdate = true;

	
	void issueJobs(); // Possibly make this to Emit a message to Job's system? // Possibly make an Entity have a queue of jobs that it needs to complete for easy mining? Or a vector of queues for each job type?

	// Checks currentJobs to make sure we haven't issued
	// select job yet
	bool jobAlreadyIssued(Job job);
};