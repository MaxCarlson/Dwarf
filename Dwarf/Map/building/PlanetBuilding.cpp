#include "stdafx.h"
#include "PlanetBuilding.h"
#include "Helpers\FastNoise.h"
#include "regionBuilder.h"
#include "Helpers\Rng.h"
#include "../World/Planet.h"
#include "../Tile.h"
#include "PlanetMapBuilder.h"

// This is the game planet
Planet planet;

void zeroPlanet()
{
	planet.tiles.resize(WORLD_WIDTH * WORLD_HEIGHT);
	std::fill(planet.tiles.begin(), planet.tiles.end(), tile_p{});
}

void buildPlanet(const std::string seed, const int pwidth, const int pheight, Coordinates mapxyz, const int water, const int plains, const int numDwarves)
{
	Rng rng(seed);
	planet.userSeed = rng.seed;
	planet.noiseSeed = rng.seed;
	planet.waterFreq = water;
	planet.plainsFreq = plains;
	planet.dwarvesNumber = numDwarves;

	WORLD_WIDTH  =  pwidth;
	WORLD_HEIGHT = pheight;

	zeroPlanet();

	region::new_region(mapxyz.x, mapxyz.y, mapxyz.z);

	FastNoise pnoise = genPlanetNoiseMap(planet);

	builPlanetTileTypes(planet);

	drawCoastlines(planet);
}
