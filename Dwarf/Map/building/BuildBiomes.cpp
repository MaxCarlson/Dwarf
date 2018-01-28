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

int findMatchingTiles(const Planet &planet, Rng &rng, int max, int x, int y, std::vector<std::pair<int, int>> &matchingTiles)
{
	const auto type = planet.tiles[planet.idx(x, y)].type;

	std::queue<std::pair<int, int>> frontier;
	std::vector<std::pair<int, int>> neighbors;
	std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> visited;

	frontier.emplace(std::make_pair(x, y));
	matchingTiles.emplace_back(std::make_pair(x, y));
	visited.emplace(std::make_pair(x, y));

	// Search outwards
	int count = 1;
	while (!frontier.empty() && count < max)
	{
		const auto t = frontier.front();
		frontier.pop();

		//visited.emplace(std::make_pair(t.first, t.second));
		findNeighbors(t.first, t.second, neighbors);

		for (const auto& n : neighbors)
		{
			auto find = visited.find(n);
			
			// If the neighbor node hasn't been visited
			if (find == visited.end())
			{
				auto& tile = planet.tiles[planet.idx(n.first, n.second)];

				// If tiles are of matching types and there's no biome there already
				if (tile.type == type && tile.biomeIdx == -1)
				{
					frontier.emplace(n);
					matchingTiles.emplace_back(n);
					visited.emplace(n);
					++count;
				}			
			}
		}
	}
	return count;
}

void findBiomeInfo(Planet& planet, const size_t bidx, const std::vector<std::pair<int, int>>& tiles)
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
		auto& tile = planet.tiles[planet.idx(t.first, t.second)];

		totalAltitude += tile.height;
		totalRainfall += tile.rainfall;
		totalVariance += tile.variance;
		totalTemp += tile.temperature;
		totalX += t.first;
		totalY += t.second;
		tile.biomeIdx = bidx;
	}

	auto& biome = planet.biomes[bidx];

	const double count = static_cast<double>(totalTiles);

	biome.avgAltitude = static_cast<int>(double(totalAltitude) / count);
	biome.avgRainfall = static_cast<int>(double(totalRainfall) / count);
	biome.avgTemperature = static_cast<int>(double(totalTemp)  / count);
	biome.avgVariance = static_cast<int>(double(totalVariance) / count);

	biome.centerX = totalX / totalTiles;
	biome.centerY = totalY / totalTiles;
}

size_t findBestBiomeMatch(const Planet& planet, int tx, int ty, const Biome& biome)
{
	int idx = -1;
	size_t bidx = 0;
	int dist = 10000;

	const auto& centerTile = planet.tiles[planet.idx(tx, ty)];

	forEachBiome([&centerTile, &biome, &dist, &idx, &bidx](BiomeDef *b) 
	{
		++idx;
		// If biome def is a possible match for our biome
		if (biome.avgTemperature >= b->minTemp && biome.avgTemperature <= b->maxTemp
			&& biome.avgRainfall >= b->minRain && biome.avgRainfall    <= b->maxRain)
		{
			bool possible = false;
			// Temp and rainfall fit, see if this biome can be placed into this planet tile type
			for (const uint8_t occurs : b->occurs)
			{
				if (occurs == centerTile.type)
				{
					possible = true;
					break;
				}
			}

			if (!possible)
				return;

			// Find the closest matching biome by distance from avg biome def
			// temperature and rainfall
			int rainCenter = (b->maxRain + b->minRain) / 2;
			int tempCenter = (b->maxTemp + b->minTemp) / 2;

			int tdist = std::abs(biome.avgTemperature - tempCenter);
			int rdist = std::abs(biome.avgRainfall - rainCenter);

			if (tdist + rdist < dist)
			{
				dist = tdist + rdist;
				bidx = idx;
			}
		}
	});

	auto b = getBiomeDef(bidx);

	bool possible = false;
	for (const auto o : b->occurs)
	{
		if (o == centerTile.type)
			possible = true;
	}

	if (!possible)
		int a = 5;

	return bidx;
}

void buildBiomes(Planet & planet, Rng & rng)
{

	const int numBiomes = WORLD_HEIGHT * WORLD_WIDTH / (8 + rng.range(1, 8));

	int filledTiles = 0;
	size_t bidx = 0;
	while (filledTiles < WORLD_HEIGHT * WORLD_WIDTH)
	{
		const int biomeSize = rng.range(35, 135);
		auto xy = std::make_pair(rng.range(0, WORLD_WIDTH - 1), rng.range(0, WORLD_HEIGHT - 1));
		std::vector<std::pair<int, int>> matchingTiles;

		int count = findMatchingTiles(planet, rng, biomeSize, xy.first, xy.second, matchingTiles);

		if (count > 0) // Possibly change this number
		{
			filledTiles += count;

			planet.biomes.push_back(Biome{});

			findBiomeInfo(planet, bidx, matchingTiles);

			size_t defIdx = findBestBiomeMatch(planet, xy.first, xy.second, planet.biomes.back());

			planet.biomes.back().type = defIdx;

			auto& tt = planet.tiles[planet.idx(xy.first, xy.second)];
			for (const auto& m : matchingTiles)
			{
				auto& t = planet.tiles[planet.idx(m.first, m.second)];
				auto b = getBiomeDef(planet.biomes[t.biomeIdx].type);

				bool possible = false;
				for (const auto& bt : b->occurs)
				{
					if (t.type == bt)
						possible = true;
				}

				if (!possible)
				{
					int a = 5;
				}

				if (t.type != tt.type)
				{
					int a = 5;
				}
			}

			updateWorldDisplay(planet);
			++bidx;
		}
	}

	int a = 5;
}
