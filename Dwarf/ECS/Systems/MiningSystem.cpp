#include "MiningSystem.h"
#include "../Tile.h"
#include "../Designations.h"
#include "JobsSystem.h"

//std::vector<uint8_t> miningMap;
//std::vector<int> miningTargets;

static const int BASE_MINING_EXP = 7;
static const int BASE_MINING_SKILL = 1;
static const double BASE_MINING_DUR = 7.5;

MiningSystem::MiningSystem(TileManager * tileManager, JobsSystem * jobsSystem) : tileManager(tileManager), jobsSystem(jobsSystem)
{
	width = tileManager->width;
	height = tileManager->height;
	depth = tileManager->depth;
	//miningMap.resize(width * depth * height);
	//miningTargets.resize(width * depth * height);
	startingPoints.reserve(200);
}

void MiningSystem::update()
{
	// No designations, no mining!!!!
	if (designations->mining.empty())
		return;

	// This should only be called if the mining map has changed due to 
	// something being mined or other events
	findStartingPoints();
}

void MiningSystem::findStartingPoints()
{
	startingPoints.clear();

	// Loop through all designations
	for (auto& des : designations->mining)
	{
		switch (des.second)
		{
		// Normal Mining
		case 1:
			regularDigging(des.first);
			break;

		default:
			break;
		}
	}

	issueJobs();
}

inline void MiningSystem::regularDigging(const Coordinates co)
{
	// This is not Ideal, eventually we'll want workers to be able to start from any position
	if (tileManager->canGo<CAN_GO_NORTH>(co)) startingPoints.push_back(std::make_pair(Coordinates{ co.x, co.y - 1, co.z }, co));
	else if (tileManager->canGo<CAN_GO_SOUTH>(co)) startingPoints.push_back(std::make_pair(Coordinates{ co.x, co.y + 1, co.z }, co));
	else if (tileManager->canGo<CAN_GO_EAST >(co)) startingPoints.push_back(std::make_pair(Coordinates{ co.x + 1, co.y, co.z }, co));
	else if (tileManager->canGo<CAN_GO_WEST >(co)) startingPoints.push_back(std::make_pair(Coordinates{ co.x - 1, co.y, co.z }, co));
}

void MiningSystem::issueJobs()
{
	for (auto& startPoint : startingPoints)
	{
		// This is too simplistic here, will have to modify all these values
		// by skill, MaterialType, distance underground, etc.
		Job newJob(startPoint.first, BASE_MINING_EXP, BASE_MINING_SKILL, BASE_MINING_DUR, Job::MINER, startPoint.second);
		jobsSystem->addJob(newJob);
	}
	
}

/*
void MiningSystem::makeMiningMap()
{
std::fill(miningMap.begin(), miningMap.end(), std::numeric_limits<uint8_t>::max());
std::fill(miningTargets.begin(), miningTargets.end(), std::numeric_limits<int>::max());

std::vector<std::tuple<int, int, int, int>> startingPoints;

for(int z = 0; z < depth; ++z)
for(int y = 0; y < height; ++y)
for (int x = 0; x < width; ++x)
{
Coordinates co = { x, y, z };
const auto idx = TILE_ARRAY_LOOKUP;
auto des = designations->mining.find(idx);

if (des != designations->mining.end())
{
switch (des->second)
{
case 1: // Regular digging
startingPoints.push_back(std::make_tuple(x - 1, y, z, idx));
startingPoints.push_back(std::make_tuple(x + 1, y, z, idx));
startingPoints.push_back(std::make_tuple(x, y - 1, z, idx));
startingPoints.push_back(std::make_tuple(x, y + 1, z, idx));
break;

// Everything after here will not be implemented for a while
case 2: // Channeling
startingPoints.push_back(std::make_tuple(x, y, z, idx));
startingPoints.push_back(std::make_tuple(x, y, z - 1, idx));
startingPoints.push_back(std::make_tuple(x - 1, y, z, idx));
startingPoints.push_back(std::make_tuple(x + 1, y, z, idx));
startingPoints.push_back(std::make_tuple(x, y - 1, z, idx));
startingPoints.push_back(std::make_tuple(x, y + 1, z, idx));
break;

case 3: // Ramping
startingPoints.push_back(std::make_tuple(x, y, z, idx));
startingPoints.push_back(std::make_tuple(x, y, z + 1, idx));
startingPoints.push_back(std::make_tuple(x - 1, y, z - 1, idx));
startingPoints.push_back(std::make_tuple(x + 1, y, z - 1, idx));
startingPoints.push_back(std::make_tuple(x, y - 1, z - 1, idx));
startingPoints.push_back(std::make_tuple(x, y + 1, z - 1, idx));
break;

case 4: // Up stairs
startingPoints.push_back(std::make_tuple(x, y, z, idx));
startingPoints.push_back(std::make_tuple(x, y, z + 1, idx));
startingPoints.push_back(std::make_tuple(x - 1, y, z, idx));
startingPoints.push_back(std::make_tuple(x + 1, y, z, idx));
startingPoints.push_back(std::make_tuple(x, y - 1, z, idx));
startingPoints.push_back(std::make_tuple(x, y + 1, z, idx));
break;

case 5: // Down stairs
startingPoints.push_back(std::make_tuple(x, y, z, idx));
startingPoints.push_back(std::make_tuple(x, y, z - 1, idx));
startingPoints.push_back(std::make_tuple(x - 1, y, z, idx));
startingPoints.push_back(std::make_tuple(x + 1, y, z, idx));
startingPoints.push_back(std::make_tuple(x, y - 1, z, idx));
startingPoints.push_back(std::make_tuple(x, y + 1, z, idx));
break;

case 6: // Up down stairs
startingPoints.push_back(std::make_tuple(x, y, z, idx));
startingPoints.push_back(std::make_tuple(x, y, z - 1, idx));
startingPoints.push_back(std::make_tuple(x, y, z + 1, idx));
startingPoints.push_back(std::make_tuple(x - 1, y, z, idx));
startingPoints.push_back(std::make_tuple(x + 1, y, z, idx));
startingPoints.push_back(std::make_tuple(x, y - 1, z, idx));
startingPoints.push_back(std::make_tuple(x, y + 1, z, idx));
break;
}
}
}

// Loop through starting points and generate
// a mining distance map
for (auto& pos : startingPoints)
walkMiningMap({ std::get<0>(pos), std::get<1>(pos), std::get<2>(pos) }, 0, std::get<3>(pos));
}

void MiningSystem::walkMiningMap(const Coordinates co, const int distance, const int idx)
{
if (distance > 200)
return;

const auto newIdx = TILE_ARRAY_LOOKUP;

if (miningMap[newIdx] > distance)
{
if (!tileManager->canWalk(co))
return;

miningMap[newIdx] = distance;
miningTargets[newIdx] = idx;

if (tileManager->canGo<CAN_GO_NORTH>(co)) walkMiningMap({ co.x, co.y - 1, co.z }, distance + 1, idx);
if (tileManager->canGo<CAN_GO_SOUTH>(co)) walkMiningMap({ co.x, co.y + 1, co.z }, distance + 1, idx);
if (tileManager->canGo<CAN_GO_EAST >(co)) walkMiningMap({ co.x + 1, co.y, co.z }, distance + 1, idx);
if (tileManager->canGo<CAN_GO_WEST >(co)) walkMiningMap({ co.x - 1, co.y, co.z }, distance + 1, idx);
if (tileManager->canGo<CAN_GO_UP   >(co)) walkMiningMap({ co.x, co.y, co.z + 1 }, distance + 1, idx);
if (tileManager->canGo<CAN_GO_DOWN >(co)) walkMiningMap({ co.x, co.y, co.z - 1 }, distance + 1, idx);
}
}
*/
