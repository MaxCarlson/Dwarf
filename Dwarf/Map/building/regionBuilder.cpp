#include "regionBuilder.h"
#include "../Coordinates.h"
#include "../Tile.h"
#include <libtcod.hpp>
#include "../Helpers/FastNoise.h"
#include "region\rocklayer.h"
#include "region\place_buildings.h"
#include "region\place_creatures.h"


const int noiseSize = 166; //def 88
const int R_SMOOTHNESS = 37; // Smoothness
constexpr unsigned int octaves = 7;
constexpr float persistence = 0.5F;
constexpr float frequency = 2.0F;


double noiseXY(int regionX, int regionXS)
{
	const double x = regionX * R_SMOOTHNESS + regionXS;
	return x / (MAP_WIDTH * R_SMOOTHNESS) * noiseSize;
}

inline uint8_t noiseToHeight(const double n)
{
	return static_cast<uint8_t>((n + 1.0f) * 103);
}

void buildHeightMap(FastNoise & noise, std::vector<uint8_t>& heightMap)
{
	heightMap.resize(MAP_HEIGHT * MAP_WIDTH, 1);
	for(int x = 0; x < MAP_WIDTH; ++x)
		for (int y = 0; y < MAP_HEIGHT; ++y)
		{
			const double nx = noiseXY(MAP_WIDTH, x);
			const double ny = noiseXY(MAP_HEIGHT, y);
			const double nz = noise.GetNoise(nx, ny);

			auto alt = noiseToHeight(nz);
			auto cellIdx = (y * MAP_WIDTH) + x;

			heightMap[cellIdx] = alt;
		}
}


void buildRegion(Coordinates dimensions)
{
	region::new_region(dimensions.x, dimensions.y, dimensions.z);

	TCODRandom * rng = TCODRandom::getInstance();

	FastNoise noise;
	noise.SetSeed(rng->getInt(1, 10000001));
	noise.SetNoiseType(FastNoise::NoiseType::SimplexFractal);
	noise.SetFractalType(FastNoise::FBM);
	noise.SetFractalOctaves(octaves);
	noise.SetFractalGain(persistence);
	noise.SetFractalLacunarity(frequency);

	std::vector<uint8_t> heightMap;
	buildHeightMap(noise, heightMap);

	Strata strata = buildStrata(heightMap, noise, *rng);
	layRock(heightMap, strata, *rng);

	placeRamps();

	region::tileRecalcAll();

	placeDwarves();
}
