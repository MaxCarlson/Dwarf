#pragma once
#include <vector>

class TCODRandom;
class FastNoise;

struct Strata
{
	std::vector<int> strata_map;
	std::vector<std::size_t> material_idx;
	std::vector<std::tuple<int, int, int, int>> counts;
};

void buildStrata(FastNoise &noise, TCODRandom & rng);
void layRock(std::vector<uint8_t> heightMapt, TCODRandom & rng);

