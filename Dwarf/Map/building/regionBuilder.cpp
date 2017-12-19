#include "regionBuilder.h"
#include "../Coordinates.h"
#include "../Tile.h"
#include <libtcod.hpp>
#include "../Helpers/FastNoise.h"
#include "region\rocklayer.h"


const int noiseSize = 88;
const int WORLD_D = 80;
constexpr unsigned int octaves = 7;
constexpr float persistence = 0.5F;
constexpr float frequency = 2.0F;

double noiseX(int regionX, int regionXS)
{
	const double x = regionX + regionXS;
	return x / MAP_WIDTH * noiseSize;
}

double noiseY(int regionX, int regionXS)
{
	const double x = regionX  + regionXS;
	return x / MAP_WIDTH  * noiseSize;
}

inline uint8_t noiseToHeight(const double n)
{
	return static_cast<uint8_t>((n + 1.0f) * 55);
}

void buildHeightMap(FastNoise & noise, std::vector<uint8_t>& heightMap)
{
	heightMap.resize(MAP_HEIGHT * MAP_WIDTH, 1);
	for(int x = 0; x < MAP_WIDTH; ++x)
		for (int y = 0; y < MAP_HEIGHT; ++y)
		{
			const double nx = noiseX(MAP_WIDTH, x);
			const double ny = noiseY(MAP_HEIGHT, y);
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

	std::vector<uint8_t> heightMapt;
	buildHeightMap(noise, heightMapt);

	
	TCODHeightMap * heightMap = new TCODHeightMap(dimensions.x, dimensions.y);
	heightMap->midPointDisplacement(rng);
	heightMap->normalize();

	layRock(*heightMap, heightMapt, *rng);
}
