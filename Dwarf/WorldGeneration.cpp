#include "stdafx.h"
#include "WorldGeneration.h"
#include "Map\building\PlanetBuilding.h"

std::unique_ptr<std::thread> worldThread;

void renderWorld()
{

}

void generateWorld(const std::string seed, const int pwidth, const int pheight, Coordinates mapxyz, const int water, const int plains, const int numDwarves)
{
	worldThread = std::make_unique<std::thread>(buildPlanet, seed, pwidth, pheight, mapxyz, water, plains, numDwarves);
}
