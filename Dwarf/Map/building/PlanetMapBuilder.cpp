#include "stdafx.h"
#include "PlanetMapBuilder.h"
#include "../World/Planet.h"
#include "../Tile.h"
#include "NoiseHelper.h"

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

	//#pragma omp parallel for
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

	//#pragma omp parallel for
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
	}
}
