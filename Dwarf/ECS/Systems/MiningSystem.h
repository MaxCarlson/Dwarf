#pragma once
#include "../Systems.h"

#include <vector>
#include <unordered_map>

class LaborStatsComponent;
class JobComponent;
struct Coordinates;

extern std::vector<uint8_t> miningMap;
extern std::vector<int> miningTargets;

class MiningSystem : public System<Requires<LaborStatsComponent, JobComponent>>
{
public:
	MiningSystem();
	//~MiningSystem();

	void init(int width_i, int height_i, int depth_i);

	//void designateMining(Coordinates topLeft, Coordinates botRight); // Make a designations class

	void makeMiningMap();
	
	//void findPick(); // Find a pickaxe before Entity starts mining (eventually)

private:
	int width, height, depth;
	void issueJob(); // Possibly make this to Emit a message to Job's system? // Possibly make an Entity have a queue of jobs that it needs to complete for easy mining? Or a vector of queues for each job type?
};

