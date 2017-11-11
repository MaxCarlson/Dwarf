#include "MapRender.h"
#include "Engine.h"
#include "Map.h"
#include "Tile.h"

#include "libtcod.hpp"
#include "BearLibTerminal.h"



MapRender::MapRender(Map & map) : map(map)
{
	// Init here doesn't work???
	//panelWidth = terminal_state(TK_WIDTH);
	//panelHeight = terminal_state(TK_HEIGHT);
}


MapRender::~MapRender()
{
}

void MapRender::render()
{
	panelWidth  = terminal_state( TK_WIDTH);
	panelHeight = terminal_state(TK_HEIGHT);

	int renderWidth  = panelWidth  + offsetX;
	int renderHeight = panelHeight + offsetY;

	for (int x = offsetX; x < renderWidth; ++x)
		for (int y = offsetY; y <  renderHeight; ++y)
		{
			const auto & t = map.tileManager.tileAt({ x, y, currentZLevel });

			// Adjusted x and y values
			// based of camera offset from top left corner
			int adjX = x - offsetX;
			int adjY = y - offsetY;

			// Use this for darkening/blacking-out unexplored in mountain/underground tiles
			if (!map.tileManager.getProperty<TileManager::EXPLORED>({ x, y, currentZLevel }))
			{

			}
			else
			{
				// Libtcod
				TCODConsole::root->setChar(adjX, adjY, t.ch);

				// BearslibTerminal
				if (t.ch == 130 || t.ch == 147 || t.ch == 244)
					terminal_color("dark green");


				terminal_put(adjX, adjY, 0xE200 + t.ch);
			}
		}

}

bool MapRender::incrementZLevel(int inc)
{
	if (currentZLevel + inc >= 0 && currentZLevel + inc < MAX_ZLVL) {
		renderMap = map.mapZLvls[currentZLevel + inc];
		currentZLevel += inc;
		return true;
	}
	return false;
}

void MapRender::jumpToZLevel(int level)
{
	if (level >= 0 && level < MAX_ZLVL) {
		renderMap = map.mapZLvls[level];
		currentZLevel = level;
	}
}


void MapRender::moveCamera(const CamDirections D)
{
	if (D == NORTH && offsetY > 0)
		offsetY -= 1;

	else if (D == SOUTH && offsetY + panelHeight < map.height)
		offsetY += 1;

	else if (D == EAST && offsetX + panelWidth < map.width)
		offsetX += 1;

	else if (D == WEST && offsetX > 0)
		offsetX -= 1;
}

