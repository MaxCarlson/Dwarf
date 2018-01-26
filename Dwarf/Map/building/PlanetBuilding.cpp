#include "stdafx.h"
#include "PlanetBuilding.h"
#include "Helpers\FastNoise.h"
#include "regionBuilder.h"
#include "Helpers\Rng.h"
#include "../World/Planet.h"
#include "../Tile.h"
#include "PlanetMapBuilder.h"
#include "WorldGeneration.h"

// This is the game planet
Planet planet;

std::mutex planetBuilderLock;
std::vector<vchar> worldGenDisplay;

void setPlanetChar(const int idx, const int tileIdx, Planet &planet)
{

	switch (planet.tiles[tileIdx].type)
	{
	case PlanetTileType::NONE:
		worldGenDisplay[idx] = { 0, 0, 0 };
		break;

	case PlanetTileType::WATER:
		worldGenDisplay[idx] = { 126, color_from_name("light blue"), color_from_name("dark blue") };
		break;

	case PlanetTileType::PLAINS:
		worldGenDisplay[idx] = { 23, color_from_name("grass_green"), color_from_name("forest_green") };
		break;

	case PlanetTileType::HILLS:
		worldGenDisplay[idx] = { 24, color_from_name("forest_green"), color_from_name("forest_green") };
		break;

	case PlanetTileType::MOUNTAINS:
		worldGenDisplay[idx] = { 30, color_from_name("grey"), color_from_name("dark grey") };
		break;

	case PlanetTileType::HIGHLANDS:
		worldGenDisplay[idx] = { 24, color_from_name("lightest grass_green"), color_from_name("brown") };
		break;

	case PlanetTileType::PLATEAUS:
		worldGenDisplay[idx] = { 24, color_from_name("light brown"), color_from_name("forest_green") };
		break;

	case PlanetTileType::COASTS:
		worldGenDisplay[idx] = { 142, color_from_name("lightest brown"), color_from_name("dark yellow") };
		break;
	}
}

void updateWorldDisplay(Planet & planet)
{
	planetBuilderLock.lock();

	for (int x = 0; x < WORLD_WIDTH; ++x)
		for (int y = 0; y < WORLD_HEIGHT; ++y)
			setPlanetChar(planet.idx(x, y), planet.idx(x, y), planet);

	planetBuilderLock.unlock();
}

void zeroPlanet()
{
	worldGenDisplay.resize(WORLD_WIDTH * WORLD_HEIGHT);
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

	//drawCoastlines(planet);

	buildPlanetBiomes(planet, rng);

	worldThread->detach();
}
