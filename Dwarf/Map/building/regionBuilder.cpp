#include "regionBuilder.h"
#include "../Coordinates.h"
#include "../Tile.h"
#include <libtcod.hpp>
#include "../Helpers/FastNoise.h"
#include "region\rocklayer.h"
#include "region\place_buildings.h"
#include "region\place_creatures.h"
#include "region\plant_trees.h"
#include "Helpers\Rng.h"
#include "NoiseHelper.h"
#include "../World/Planet.h"
#include "Raws\Defs\BiomeDef.h"
#include "Raws\BiomeReader.h"

void buildHeightMap(FastNoise & noise, std::vector<uint8_t>& heightMap)
{
	heightMap.resize(MAP_HEIGHT * MAP_WIDTH, 1);
	for(int x = 0; x < MAP_WIDTH; ++x)
		for (int y = 0; y < MAP_HEIGHT; ++y)
		{
			const double nx = noiseX(MAP_WIDTH  / 2, x);
			const double ny = noiseX(MAP_HEIGHT / 2, y);
			const double nz = noise.GetNoise(nx, ny);

			auto alt = noiseToHeight(nz);
			auto cellIdx = (y * MAP_WIDTH) + x;

			heightMap[cellIdx] = alt;
		}
}

std::pair<Biome, BiomeDef> getRegionBiome(Planet &planet, int px, int py)
{
	const int idx = planet.idx(px, py);
	const int biomeIdx = planet.tiles[idx].biomeIdx;
	return std::make_pair(planet.biomes[biomeIdx], *getBiomeDef(planet.biomes[biomeIdx].type));
}

void buildRegion(Planet &planet, int px, int py, Coordinates dimensions, Rng &rng)
{
	region::new_region(dimensions.x, dimensions.y, dimensions.z, planet.tiles[planet.idx(px, py)].biomeIdx);

	auto biomeInfo = getRegionBiome(planet, px, py);

	FastNoise noise;
	noise.SetSeed(planet.noiseSeed);
	noise.SetNoiseType(FastNoise::NoiseType::SimplexFractal);
	noise.SetFractalType(FastNoise::FBM);
	noise.SetFractalOctaves(octaves);
	noise.SetFractalGain(persistence);
	noise.SetFractalLacunarity(frequency);

	std::vector<uint8_t> heightMap;
	buildHeightMap(noise, heightMap);

	Strata strata = buildStrata(heightMap, noise, rng);
	layRock(heightMap, biomeInfo.second, strata, rng);

	placeRamps();

	plantTrees(biomeInfo.second.deciduosChance, biomeInfo.second.evergreenChance, rng);

	region::tile_recalc_all();

	placeDwarves();
}
