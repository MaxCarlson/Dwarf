#include "stdafx.h"
#include "BuildBiomes.h"
#include "../World/Planet.h"
#include "Helpers\Rng.h"
#include "Raws\BiomeReader.h"
#include "Raws\Defs\BiomeDef.h"
#include "Map\building\PlanetBuilding.h"
#include <boost\functional\hash.hpp>

inline void findNeighbors(int x, int y, std::vector<std::pair<int, int>> &neighbors)
{
	neighbors.clear();

	if (x < WORLD_WIDTH - 1)
		neighbors.emplace_back(std::make_pair(x + 1, y));
	if (x > 0)
		neighbors.emplace_back(std::make_pair(x - 1, y));
	if (y < WORLD_HEIGHT - 1)
		neighbors.emplace_back(std::make_pair(x, y + 1));
	if (y > 0)
		neighbors.emplace_back(std::make_pair(x, y - 1));
}

int findMatchingTiles(Planet &planet, Rng &rng, int max, int x, int y, std::vector<std::pair<int, int>> &matchingTiles)
{
	auto& tile = planet.tiles[planet.idx(x, y)];
	const auto type = tile.type;

	std::queue<std::pair<int, int>> frontier;
	std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> visited;
	static std::vector<std::pair<int, int>> neighbors;

	frontier.emplace(std::make_pair(x, y));

	// Search outwards
	int count = 1;
	while (!frontier.empty() && count < max)
	{
		const auto t = frontier.front();
		frontier.pop();

		visited.emplace(std::make_pair(t.first, t.second));
		findNeighbors(t.first, t.second, neighbors);

		for (const auto& n : neighbors)
		{
			auto find = visited.find(n);
			
			// If the neighbor node hasn't been visited
			if (find == visited.end())
			{
				tile = planet.tiles[planet.idx(n.first, n.second)];

				// If tiles are of matching types
				if (tile.type == type)
				{
					frontier.emplace(n);
					matchingTiles.emplace_back(n);
					++count;
				}			
			}
		}
	}
	return count;
}

void findBiomeInfo(Planet & planet, const size_t bidx, const std::vector<std::pair<int, int>>& tiles)
{
	const int totalTiles = tiles.size();
	int totalAltitude = 0;
	int totalRainfall = 0;
	int totalVariance = 0;
	int totalTemp = 0;
	int totalX = 0;
	int totalY = 0;

	for (const auto& t : tiles)
	{
		const auto& tile = planet.tiles[planet.idx(t.first, t.second)];

		totalAltitude += tile.height;
		totalRainfall += tile.rainfall;
		totalVariance += tile.variance;
		totalTemp += tile.temperature;
		totalX += t.first;
		totalY += t.second;
	}

	auto & biome = planet.biomes[bidx];

	const double count = static_cast<double>(totalTiles);

	biome.avgAltitude = static_cast<uint8_t>(static_cast<double>(totalAltitude) / count);
	biome.avgRainfall = static_cast<uint8_t>(static_cast<double>(totalRainfall) / count);
	biome.avgTemperature = static_cast<uint8_t>(static_cast<double>(totalTemp)  / count);
	biome.avgVariance = static_cast<uint8_t>(static_cast<double>(totalVariance) / count);

	biome.centerX = totalX / totalTiles;
	biome.centerY = totalY / totalTiles;
}

size_t findBestBiomeMatch(const Biome& biome)
{
	size_t idx = 0;
	size_t bidx = 0;
	int dist = 10000;

	forEachBiome([&biome, &dist, &idx, &bidx](BiomeDef *b) 
	{
		// If biome def is a possible match for our biome
		if (biome.avgTemperature >= b->minTemp && biome.avgTemperature <= b->maxTemp
			&& biome.avgRainfall >= b->minRain && biome.avgRainfall <= b->maxRain)
		{

			// Find the closest matching biome by distance from avg biome def
			// temperature and rainfall
			int rainCenter = std::abs(b->maxTemp - b->minTemp);
			int tempCenter = std::abs(b->maxTemp - b->minTemp);

			int tdist = std::abs(biome.avgTemperature - tempCenter);
			int rdist = std::abs(biome.avgRainfall - rainCenter);

			if (tdist + rdist < dist)
			{
				dist = tdist + rdist;
				bidx = idx;
			}
		}
		++idx;
	});

	return bidx;
}

void buildBiomes(Planet & planet, Rng & rng)
{

	const int numBiomes = WORLD_HEIGHT * WORLD_WIDTH / (8 + rng.range(1, 8));

	int filledTiles = 0;

	while (filledTiles < WORLD_HEIGHT * WORLD_WIDTH)
	{
		const int biomeSize = rng.range(35, 105);
		auto xy = std::make_pair(rng.range(0, WORLD_WIDTH - 1), rng.range(0, WORLD_HEIGHT - 1));
		std::vector<std::pair<int, int>> matchingTiles;

		int count = findMatchingTiles(planet, rng, biomeSize, xy.first, xy.second, matchingTiles);

		if (count > 6) // Posibbly change this number
		{
			filledTiles += count;

			planet.biomes.emplace_back(Biome{});

			findBiomeInfo(planet, planet.biomes.size() - 1, matchingTiles);

			size_t defIdx = findBestBiomeMatch(planet.biomes.back());

			planet.biomes.back().type = defIdx;

			updateWorldDisplay(planet);
		}
	}

}
