#include "stdafx.h"
#include "WorldGeneration.h"
#include "Map\building\PlanetBuilding.h"
#include "Map\World\Planet.h"

std::unique_ptr<std::thread> worldThread;

void renderWorld()
{
	terminal_clear();
	int count = 0;
	int x = 0;
	int y = 0;
	for (const auto& v : worldGenDisplay)
	{
		terminal_color(v.fg);
		terminal_bkcolor(v.bg);
		terminal_put(x, y, 0xE100 + v.c);

		++x;

		if (x > WORLD_WIDTH - 1)
		{
			x = 0;
			++y;
		}
	}

	terminal_refresh();
}

void generateWorld(const std::string seed, const int pwidth, const int pheight, Coordinates mapxyz, const int water, const int plains, const int numDwarves)
{
	worldThread = std::make_unique<std::thread>(buildPlanet, seed, pwidth, pheight, mapxyz, water, plains, numDwarves);
	worldThread->detach();

	while (!worldThread->joinable())
	{
		renderWorld();
	}

	int a = 55;
}
