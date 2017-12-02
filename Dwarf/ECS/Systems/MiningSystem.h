#pragma once
#include "../Systems.h"

#include <vector>
#include <unordered_map>


class TileManager;
struct Coordinates;
class TileFactory;

extern std::vector<uint8_t> miningMap;
extern std::vector<int> miningTargets;

class MiningSystem : public System<Requires<>> // Does not interate through Entities
{
public:
	MiningSystem() = default;

	void init();
	void update();
	
	void makeMiningMap();	

	void performMining(Entity e, const int targetIdx, const uint8_t miningType);

private:
	void walkMiningMap(const Coordinates co, const int distance, const int idx);
};