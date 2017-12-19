#pragma once
#include <vector>
#include <tuple>

class TCODRandom;
class FastNoise;

struct Strata
{
	std::vector<int> strata_map;
	std::vector<std::size_t> material_idx;
	std::vector<std::tuple<int, int, int, int>> counts;
};

Strata buildStrata(std::vector<uint8_t> &heightMap, FastNoise &noise, TCODRandom & rng);
void layRock(std::vector<uint8_t> heightMap, Strata & strata, TCODRandom & rng);

