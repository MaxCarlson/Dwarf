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

			for(int x1 = 0; x1 < MAP_WIDTH / MAP_FRACTION; ++x1)
				for (int y1 = 0; y1 < MAP_HEIGHT / MAP_FRACTION; ++y1)
				{
					const auto nz = noise.GetNoise(noiseX(x, x1 * MAP_FRACTION), noiseY(y, y1 * MAP_FRACTION));
				}
		}
	}
}
