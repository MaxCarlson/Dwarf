#include "stdafx.h"
#include "PlanetMapBuilder.h"
#include "../World/Planet.h"
#include "../Tile.h"
#include "NoiseHelper.h"
#include "Helpers\Rng.h"
#include "Raws\BiomeReader.h"
#include "Raws\Defs\BiomeDef.h"
#include "PlanetBuilding.h"

FastNoise genPlanetNoiseMap(Planet & planet)
{
	FastNoise noise(planet.noiseSeed);
	noise.SetNoiseType(FastNoise::NoiseType::SimplexFractal);
	noise.SetFractalType(FastNoise::FractalType::FBM);
	noise.SetFractalOctaves(5U);
	noise.SetFractalGain(0.5F);
	noise.SetFractalLacunarity(2.0F);

	constexpr double maxTemp = 100.0;
	constexpr double minTemp = -100.0;
	constexpr double range = maxTemp - minTemp;
	const double halfHeight = WORLD_HEIGHT / 2.0;

	#pragma omp parallel for
	for (int y = 0; y < WORLD_HEIGHT; ++y)
	{
		const auto distanceFromEquator = std::abs(WORLD_HEIGHT / 2 - y);
		const auto tempRange = 1.0 - (static_cast<double>(distanceFromEquator) / halfHeight);
		const auto tempByLatitude = (tempRange * range) + minTemp;

		for (int x = 0; x < WORLD_WIDTH; ++x)
		{
			
			uint8_t min = 0;
			uint8_t max = std::numeric_limits<uint8_t>::max();
			int totalHeight = 0;
			int numTiles = 0;

			for(int x1 = 0; x1 < MAP_WIDTH / MAP_FRACTION; ++x1)
				for (int y1 = 0; y1 < MAP_HEIGHT / MAP_FRACTION; ++y1)
				{
					const auto nz = noise.GetNoise(noiseX(x, x1 * MAP_FRACTION), noiseY(y, y1 * MAP_FRACTION));

					auto n = noiseToHeight(nz);
					if (n < min) n = min;
					if (n > max) n = max;
					totalHeight += n;
					++numTiles;
				}
			auto& tile = planet.tiles[planet.idx(x, y)];

			tile.height = static_cast<uint8_t>(totalHeight / numTiles);
			tile.type = 0;
			tile.variance = max - min;

			const double altChange = std::abs(tile.height - planet.waterLevel) / 10.0;
			tile.temperature = tempByLatitude - altChange;

			// Normalize temperature here
		}

		updateWorldDisplay(planet);
	}

	return noise;
}

int determineNumberOfTiles(Planet &planet, int &tiles, int target)
{
	int count = 0;
	while (count < target)
	{
		const int count = std::count_if(planet.tiles.begin(), planet.tiles.end(), [tiles](const tile_p &t)
		{
			return t.height <= tiles;
		});

		if (count >= target)
			return tiles;
		else
			++tiles;
	}
	throw std::runtime_error("determineNumberOfTiles for planet Error!");
}

void builPlanetTileTypes(Planet & planet)
{
	int candidate = 0;
	const int remaining = 10 - planet.waterFreq + planet.plainsFreq;
	const int numTiles = WORLD_WIDTH * WORLD_HEIGHT;
	const int waterTiles = numTiles / planet.waterFreq;
	const int plainsTiles = numTiles / planet.plainsFreq + waterTiles;
	const int hillsTiles = numTiles / remaining + plainsTiles;

	planet.waterLevel = determineNumberOfTiles(planet, candidate, waterTiles);
	planet.plainsHeight = determineNumberOfTiles(planet, candidate, plainsTiles);
	planet.hillsHeight = determineNumberOfTiles(planet, candidate, hillsTiles);

	#pragma omp parallel for
	for (auto i = 0; i < planet.tiles.size(); ++i)
	{
		auto& tile = planet.tiles.at(i);
		if (tile.height < planet.waterLevel)
		{
			tile.type = PlanetTileType::WATER;
			tile.rainfall = 15;
		}
		else if (tile.height <= planet.plainsHeight)
		{
			tile.type = PlanetTileType::PLAINS;
			tile.rainfall = 10;
		}
		else if (tile.height <= planet.hillsHeight)
		{
			tile.type = PlanetTileType::HILLS;
			tile.rainfall = 17;
			if (tile.variance < 2)
			{
				tile.type = PlanetTileType::HIGHLANDS;
				tile.rainfall = 10;
			}
		}
		else
		{
			tile.type = PlanetTileType::MOUNTAINS;
			tile.rainfall = 30;
			if (tile.variance < 3)
			{
				tile.type = PlanetTileType::PLATEAUS;
				tile.rainfall = 15;
			}
		}
		updateWorldDisplay(planet);
	}
}

void drawCoastlines(Planet & planet)
{
	for(int x = 1; x < WORLD_WIDTH - 1; ++x)
		for (int y = 1; y < WORLD_HEIGHT - 1; ++y)
		{
			auto& tile = planet.tiles[planet.idx(x, y)];

			if (tile.type > PlanetTileType::WATER)
			{
				if (planet.tiles[planet.idx(x - 1, y)].type == PlanetTileType::WATER ||
					planet.tiles[planet.idx(x + 1, y)].type == PlanetTileType::WATER ||
					planet.tiles[planet.idx(x, y - 1)].type == PlanetTileType::WATER ||
					planet.tiles[planet.idx(x, y + 1)].type == PlanetTileType::WATER ||
					planet.tiles[planet.idx(x + 1, y - 1)].type == PlanetTileType::WATER ||
					planet.tiles[planet.idx(x - 1, y - 1)].type == PlanetTileType::WATER ||
					planet.tiles[planet.idx(x + 1, y + 1)].type == PlanetTileType::WATER ||
					planet.tiles[planet.idx(x - 1, y - 1)].type == PlanetTileType::WATER )
				{
					tile.type = PlanetTileType::COASTS;
					tile.rainfall = 20;
				}
			}
			updateWorldDisplay(planet);
		}
}

void buildPlanetRainfall(Planet & planet)
{
	#pragma omp parallel for
	for (int y = 0; y < WORLD_HEIGHT; ++y)
	{
		int rainAmt = 10;
		for (int x = 0; x < WORLD_WIDTH; ++x)
		{
			auto& tile = planet.tiles[planet.idx(x, y)];

			if (tile.type == PlanetTileType::MOUNTAINS)
				rainAmt -= 20;

			else if (tile.type == PlanetTileType::MOUNTAINS)
				rainAmt -= 10;

			else if (tile.type == PlanetTileType::MOUNTAINS)
				rainAmt -= 5;

			else
				rainAmt += 1;

			if (rainAmt < 0) rainAmt = 0;
			if (rainAmt > 20) rainAmt = 20;

			tile.rainfall += rainAmt;

			if (tile.rainfall < 0)  tile.rainfall = 0;
			if (tile.rainfall > 100) tile.rainfall = 100;
		}
	}
}

std::unordered_map<uint8_t, double> determineBiomeConstituants(Planet &planet, const size_t bidx)
{
	std::unordered_map<uint8_t, double> percents;
	std::unordered_map<uint8_t, long>   tcounts;

	int numTiles = 0;
	int totalAltitude = 0;
	int totalRainfall = 0;
	int totalVariance = 0;
	int totalTemp = 0;
	int totalX = 0;
	int totalY = 0;


	// Find all tiles belonging to biome 
	// and count all details about them so we can find averages
	// and assign them to the biome
	for(int x = 0; x < WORLD_WIDTH; ++x)
		for (int y = 0; y < WORLD_HEIGHT; ++y)
		{
			const int tidx = planet.idx(x, y);
			const auto& tile = planet.tiles[tidx];

			if (tile.biomeIdx == bidx)
			{
				++numTiles;
				totalX += x;
				totalY += y;
				totalAltitude += tile.height;
				totalRainfall += tile.rainfall;
				totalTemp     += tile.temperature;
				totalVariance += tile.variance;

				auto find = tcounts.find(tile.type);

				if (find == tcounts.end())
					tcounts[tile.type] = 1L;
				else
					++tcounts[tile.type];
			}
		}

	if (numTiles == 0)
		numTiles = 1;

	auto& biome = planet.biomes[bidx];

	// Assign averages to biome
	double count = static_cast<double>(numTiles);

	biome.avgAltitude = static_cast<double>(totalAltitude) / count;
	biome.avgRainfall = static_cast<double>(totalRainfall) / count;
	biome.avgTemperature = static_cast<double>(totalTemp)  / count;
	biome.avgVariance = static_cast<double>(totalVariance) / count;

	biome.centerX = totalX / numTiles;
	biome.centerY = totalY / numTiles;

	// Find out how many of each tile type is contained in the biome
	for (int i = 0; i < PlanetTileType::MAX_PLANET_TILE_TYPES; ++i)
	{
		auto find = tcounts.find(i);

		if (find == tcounts.end())
			percents[i] = 0.0;
		else
		{
			double pct = static_cast<double>(find->second) / count;
			percents[i] = pct;
		}
	}

	return percents;
}

std::vector<std::pair<double, size_t>> findPossibleBiomes(std::unordered_map<uint8_t, double> &percents, const Biome &biome)
{
	std::vector<std::pair<double, size_t>> ret;

	// Find biomes matching the generated biome 
	size_t idx = 0;
	forEachBiome([&biome, &idx, &percents, &ret](BiomeDef *b)
	{
		if (   biome.avgTemperature >= b->minTemp && biome.avgTemperature <= b->maxTemp
			&& biome.avgRainfall    >= b->minRain && biome.avgRainfall    <= b->maxRain)
		{
			
			// Temp and rainfall fit, see if tiletypes are availible
			for (const uint8_t occurs : b->occurs)
			{
				auto find = percents.find(occurs);

				if (find != percents.end() && find->second > 0)
					ret.emplace_back(std::make_pair(find->second * 100.0, idx));
			}
		}
		++idx;
	});

	return ret;
}

void buildPlanetBiomes(Planet & planet, Rng & rng)
{
	const int numBiomes = WORLD_HEIGHT * WORLD_WIDTH / (32 + rng.range(1, 64));

	std::vector<std::pair<int, int>> biomeIndicies;

	// Randomly place biomes
	for (int i = 0; i < numBiomes; ++i)
	{
		biomeIndicies.emplace_back(std::make_pair(rng.range(1, WORLD_WIDTH), rng.range(1, WORLD_HEIGHT)));
		planet.biomes.emplace_back(Biome{});
	}

	// Asign each tile a biome identity based on closest to
	for (int x = 0; x < WORLD_WIDTH; ++x)
	{
		for (int y = 0; y < WORLD_HEIGHT; ++y)
		{
			int tileDist = std::numeric_limits<int>::max();
			int biomeIdx = -1;

			for (int i = 0; i < numBiomes; ++i)
			{
				const int biomeX = biomeIndicies[i].first;
				const int biomeY = biomeIndicies[i].second;

				const int biomeDist = get_2D_distance({ biomeX, biomeY, 1 }, { x, y, 1 });

				if (biomeDist < tileDist)
				{
					tileDist = biomeDist;
					biomeIdx = i;
				}
			}

			planet.tiles[planet.idx(x, y)].biomeIdx = biomeIdx;
		}
		updateWorldDisplay(planet);
	}


	size_t count = 0;
	size_t noMatch = 0;

	for (auto &biome : planet.biomes)
	{
		// Determine info about the biome
		auto membershipCount = determineBiomeConstituants(planet, count);

		if (!membershipCount.empty())
		{
			// Find possible matching biomes from biome defs lua
			auto possibleTypes = findPossibleBiomes(membershipCount, biome);

			if (!possibleTypes.empty())
			{
				double max = 0.0;
				for (const auto &possible : possibleTypes)
					max += possible.first;

				int roll = rng.range(1, static_cast<int>(max));

				for (const auto& possible : possibleTypes)
				{
					roll -= static_cast<int>(possible.first);
					if (roll < 0)
					{
						biome.type = possible.second;
						break;
					}
				}

				if (biome.type == -1)
					biome.type = possibleTypes[possibleTypes.size() - 1].second;

				// Name biomes eventually?
			}
			else
				++noMatch;
		}
		updateWorldDisplay(planet);
		++count;
	}
}
