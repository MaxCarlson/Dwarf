#pragma once
#include <vector>

class TCODHeightMap;
class TCODRandom;

struct Strata
{
	std::vector<int> strata_map;
	std::vector<std::size_t> material_idx;
	std::vector<std::tuple<int, int, int, int>> counts;
};

void layRock(const TCODHeightMap& heightMap, std::vector<uint8_t> heightMapt, TCODRandom & rng);

