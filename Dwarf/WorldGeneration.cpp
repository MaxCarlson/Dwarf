#include "stdafx.h"
#include "WorldGeneration.h"
#include "Map\building\PlanetBuilding.h"
#include "Map\World\Planet.h"
#include "Drawing\draw.h"

std::unique_ptr<std::thread> worldThread;

constexpr int worldOffsetX = 7;
constexpr int worldOffsetY = 0;

void renderWorld(bool drawCursor = false, int cx = 0, int cy = 0)
{
	terminal_clear();
	int count = 0;
	int x = worldOffsetX;
	int y = worldOffsetY;
	for (const auto& v : worldGenDisplay)
	{

		terminal_put(x, y, 0xE100 + v.c);

		if (drawCursor && cx == x && cy == y)
		{
			terminal_color("grey");
			terminal_put(x, y, 0xE100 + 88);
		}
		else
		{
			terminal_color(v.fg);
			terminal_bkcolor(v.bg);
		}
			

		++x;

		if (x > WORLD_WIDTH - 1 + worldOffsetX)
		{
			x = worldOffsetX;
			++y;
		}
	}

	terminal_refresh();
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
	using namespace draw;

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
