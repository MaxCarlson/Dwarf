#include "MapRender.h"
#include "../Engine.h"
#include "Map.h"
#include "Tile.h"

#include "libtcod.hpp"
#include "../BearLibTerminal.h"



MapRender::MapRender(Map & map) : map(map)
{
}

MapRender::~MapRender()
{
}

// Move all this stuff to render system?
// force render system to read from a vector of entities stored at z level?
void MapRender::render()
{
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
			const auto & t = tileManager->tileAt({ x, y, currentZLevel });

			// Adjusted x and y values
			// based of camera offset from top left corner
			int adjX = x - offsetX;
			int adjY = y - offsetY;

			// Use this for darkening/blacking-out unexplored in mountain/underground tiles
			if (!tileManager->getProperty<Tile::EXPLORED>({ x, y, currentZLevel }))
			{

			}
			else
			{
				terminal_color(t.color);
				terminal_put(adjX, adjY, 0xE200 + t.ch);
			}
		}

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

	// Adjust for Gui size  + engine.gui.verticalSize ???
	else if (D == SOUTH && offsetY + panelHeight < map.height)
		offsetY += 1;

	// Adjust for gui size
	else if (D == EAST && offsetX + panelWidth < map.width + engine.gui.horizontalOffset)
		offsetX += 1;

	else if (D == WEST && offsetX > 0)
		offsetX -= 1;
}

