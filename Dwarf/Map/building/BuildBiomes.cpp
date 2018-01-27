#include "stdafx.h"
#include "BuildBiomes.h"
#include "../World/Planet.h"
#include "Helpers\Rng.h"

inline void findNeighbors(int x, int y, std::vector<std::pair<int, int>> &neighbors)
{
	neighbors.clear();

	if (x < WORLD_WIDTH - 1)
		neighbors.emplace_back(std::make_pair(x + 1, y));
}

void findMatchingTiles(Planet &planet, Rng &rng, int max, int x, int y, std::vector<std::pair<int, int>> &matchingTiles)
{
	const auto& tile = planet.tiles[planet.idx(x, y)];
	const auto type = tile.type;

	std::unordered_set<std::pair<int, int>> visited;
	visited.emplace(std::make_pair(x, y));

	// Search outwards
	int count = 1;
	static std::vector<std::pair<int, int>> neighbors;

	std::queue<std::pair<int, int>> frontier;
	frontier.emplace(std::make_pair(x, y));

	while (!frontier.empty() && count < max)
	{
		const auto t = frontier.front();
		frontier.pop();

		visited.insert(std::make_pair(t.first, t.second));
		findNeighbors(t.first, t.second, neighbors);

		for (const auto& n : neighbors)
		{
			auto find = visited.find(n);
			
			// If the neighbor node hasn't been visited
			if (find == visited.end())
			{
				frontier.emplace(n);
			}
		}
	}
}

void buildBiomes(Planet & planet, Rng & rng)
{
	constexpr int biomeSize = 45;

	const int numBiomes = WORLD_HEIGHT * WORLD_WIDTH / (8 + rng.range(1, 8));

	int filledTiles = 0;

	while (filledTiles < WORLD_HEIGHT * WORLD_WIDTH)
	{
		auto xy = std::make_pair(rng.range(1, WORLD_WIDTH), rng.range(1, WORLD_HEIGHT));
		std::vector<std::pair<int, int>> matchingTiles;

		findMatchingTiles(planet, rng, biomeSize, xy.first, xy.second, matchingTiles);
	}

}
