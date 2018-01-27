#include "stdafx.h"
#include "WorldGeneration.h"
#include "Map\building\PlanetBuilding.h"
#include "Map\World\Planet.h"
#include "Drawing\draw.h"
#include "Map\Tile.h"
#include "Raws\BiomeReader.h"
#include "Raws\Defs\BiomeDef.h"

std::unique_ptr<std::thread> worldThread;

constexpr int worldOffsetX = 7;
constexpr int worldOffsetY = 0;

inline void printBiomeInfo(const int x, const int y)
{
	int dx = 10000, dy = 10000;

	double dist = 100000.0;

	// Find closest biome to cursor
	int i = 0;
	int pbidx = 0;
	for (const auto& b : planet.biomes)
	{
		double d = get_2D_distance({ x, y, 1 }, { b.centerX, b.centerY, 1 });
		if (d < dist)
		{
			dist = 3;
			pbidx = i;
		}
		++i;
	}

	auto bidx = planet.biomes[pbidx].type;
	auto biomeName = getBiomeDef(bidx)->name;

	const auto& tile = planet.tiles[planet.idx(x, y)];
	
	terminal_print_ext(worldOffsetX, WORLD_HEIGHT + worldOffsetY, 0, 0, TK_ALIGN_LEFT, biomeName.c_str());
	terminal_print_ext(worldOffsetX, WORLD_HEIGHT + worldOffsetY + 1, 0, 0, TK_ALIGN_LEFT, (std::string("Temperature ") + std::to_string(tile.temperature)).c_str());
	terminal_print_ext(worldOffsetX, WORLD_HEIGHT + worldOffsetY + 2, 0, 0, TK_ALIGN_LEFT, (std::string("Rainfall ") + std::to_string(tile.rainfall)).c_str());
	terminal_print_ext(worldOffsetX, WORLD_HEIGHT + worldOffsetY + 3, 0, 0, TK_ALIGN_LEFT, (std::string("Height ") + std::to_string(tile.height)).c_str());
	terminal_print_ext(worldOffsetX, WORLD_HEIGHT + worldOffsetY + 4, 0, 0, TK_ALIGN_LEFT, (std::string("Type ") + std::to_string(tile.type)).c_str());
	terminal_print_ext(worldOffsetX, WORLD_HEIGHT + worldOffsetY + 5, 0, 0, TK_ALIGN_LEFT, (std::string("Variance ") + std::to_string(tile.variance)).c_str());
}

void renderWorld(bool drawCursor = false, int cx = 0, int cy = 0)
{
	terminal_clear();
	terminal_composition(false);
	int count = 0;
	int x = worldOffsetX;
	int y = worldOffsetY;
	for (const auto& v : worldGenDisplay)
	{
		//terminal_bkcolor("grey");
		terminal_color(v.fg);
		terminal_bkcolor(v.bg);
		terminal_put(x, y, 0xE100 + v.c);

		if (drawCursor && cx == x && cy == y)
		{
			terminal_color("grey");
			terminal_put(x, y, 0xE100 + 88);

			printBiomeInfo(x, y);
		}

		++x;

		if (x > WORLD_WIDTH - 1 + worldOffsetX)
		{
			x = worldOffsetX;
			++y;
		}
	}

	terminal_refresh();
	terminal_composition(true);
}

void generateWorld(const std::string seed, const int pwidth, const int pheight, Coordinates mapxyz, const int water, const int plains, const int numDwarves)
{
	worldThread = std::make_unique<std::thread>(buildPlanet, seed, pwidth, pheight, mapxyz, water, plains, numDwarves);

	while (worldThread->joinable())
	{
		renderWorld();
	}
}

bool handleIn(int &x, int &y, bool &quit)
{
	int in = terminal_read();

	if (in == TK_RIGHT)
		++x;
	else if (in == TK_LEFT)
		--x;
	else if (in == TK_UP)
		--y;
	else if (in == TK_DOWN)
		++y;

	if (y > WORLD_HEIGHT + worldOffsetY)
		y = WORLD_HEIGHT;
	else if (y < 0 + worldOffsetY)
		y = worldOffsetY;

	if (x > WORLD_WIDTH + worldOffsetX)
		x = WORLD_WIDTH;
	else if (x < 0 + worldOffsetX)
		x = worldOffsetX;

	if (in == TK_ENTER)
		return true;

	if (in == TK_ESCAPE)
		quit = true;

	return false;
}

bool chooseEmbark(int &px, int &py)
{
	//using namespace draw;

	int x = WORLD_WIDTH  / 2 + worldOffsetX;
	int y = WORLD_HEIGHT / 2 + worldOffsetY;

	bool quit = false;
	bool chosen = false;
	while (!chosen)
	{
		renderWorld(true, x, y);
		chosen = handleIn(x, y, quit);

		if (quit)
			return false;

		if (chosen)
		{
			px = x;
			py = y;
			return true;
		}
	}

	return false;
}
