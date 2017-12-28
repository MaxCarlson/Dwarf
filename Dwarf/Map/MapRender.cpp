#include "MapRender.h"
#include "../Engine.h"
#include "Map.h"
#include "Tile.h"

#include "libtcod.hpp"
#include "../BearLibTerminal.h"
#include "../Raws/Materials.h"
#include "../Raws/Defs/MaterialDef.h"

using namespace region;

MapRender::MapRender()
{
	currentZLevel = MAP_DEPTH - 1;
}


// Move all this stuff to render system?
// force render system to read from a vector of entities stored at z level?
void MapRender::render()
{
	/*
	// Grab window size data
	panelWidth  = terminal_state( TK_WIDTH);
	panelHeight = terminal_state(TK_HEIGHT);

	// Calculate offsets so we're not rendering
	// out of screen tiles or into the gui
	int renderWidth  = panelWidth  + offsetX; // Use gui offset if rendering becomes a resource drain!!
	int renderHeight = panelHeight + offsetY;

	for (int x = offsetX; x < renderWidth; ++x)
		for (int y = offsetY; y < renderHeight; ++y)
		{
			// Adjusted x and y values
			// based of camera offset from top left corner
			int adjX = x - offsetX;
			int adjY = y - offsetY;


			//terminal_color(t.color);
			//terminal_put(adjX, adjY, 0xE200 + t.ch);

			auto* mat = getMaterial(region::getTileMaterial({ x, y, currentZLevel }));

			const int idx = getIdx({ x, y, currentZLevel });
			auto tileType = getTileType(idx);

			terminal_color(mat->color.c_str());

			switch (tileType)
			{
			case TileTypes::EMPTY_SPACE:
				terminal_color("black");
				terminal_put(adjX, adjY, 0xE200 + 0);
				break;

			case TileTypes::SOLID:
				terminal_put(adjX, adjY, 0xE200 + mat->charCode);
				break;

			case TileTypes::FLOOR:
				terminal_put(adjX, adjY, 0xE200 + mat->floorCode);
				break;

			case TileTypes::WALL: // Not implemented yet

				break;

			case TileTypes::RAMP:
				terminal_put(adjX, adjY, 0xE200 + 30);
				break;
			}		
		}
	*/
}

bool MapRender::incrementZLevel(int inc)
{
	if (currentZLevel + inc >= 0 && currentZLevel + inc < MAX_ZLVL) {
		currentZLevel += inc;
		return true;
	}
	return false;
}

void MapRender::jumpToZLevel(int level)
{
	if (level >= 0 && level < MAX_ZLVL) {
		currentZLevel = level;
	}
}

// Worth making a template?
void MapRender::moveCamera(const CamDirections D)
{
	if (D == NORTH && offsetY > 0)
		offsetY -= 1;

	// Adjust for Gui size  + engine->gui.verticalSize ???
	else if (D == SOUTH && offsetY + panelHeight < MAP_HEIGHT)
		offsetY += 1;

	// Adjust for gui size
	else if (D == EAST && offsetX + panelWidth < MAP_WIDTH + engine->gui.horizontalOffset)
		offsetX += 1;

	else if (D == WEST && offsetX > 0)
		offsetX -= 1;
}

