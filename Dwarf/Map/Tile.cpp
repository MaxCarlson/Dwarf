#include "Tile.h"
#include <memory>

// External map info
int MAP_WIDTH, MAP_HEIGHT, MAP_DEPTH, TOTAL_MAP_TILES;

namespace region
{
	// Creates a 1D Vector of Tile objects used to
	// simulate a 3D area of tiles. Access Tiles through here
	struct Region
	{
		Region()
		{
			tileMap.resize(TOTAL_MAP_TILES);
		}

		// 1D vector of Tiles indexed by 3D formula
		std::vector<Tile> tileMap;
	};


	std::unique_ptr<Region> currentRegion;


	int getIdx(Coordinates co)
	{
		if (co.z >= MAP_DEPTH || co.z < 0 || co.y >= MAP_HEIGHT || co.y < 0 || co.x >= MAP_WIDTH || co.x < 0)
			return 0;

		return TILE_ARRAY_LOOKUP;
	}

	Coordinates idxToCo(int idx)
	{
		int z = idx / (MAP_HEIGHT * MAP_WIDTH);
		idx -= (z * MAP_WIDTH * MAP_HEIGHT);

		int y = idx / MAP_WIDTH;
		idx -= (y * MAP_WIDTH);

		int x = idx;

		return { x, y, z };
	}

	int get_rough_distance(Coordinates loc, Coordinates dest)
	{
		int x = dest.x - loc.x;
		int y = dest.y - loc.y;
		int z = dest.z - loc.z;
		return (x * x + y * y + z * z);
	}

	void new_region(int width, int height, int depth)
	{
		MAP_WIDTH = width;
		MAP_HEIGHT = height;
		MAP_DEPTH = depth;
		TOTAL_MAP_TILES = MAP_WIDTH * MAP_HEIGHT * MAP_DEPTH;

		currentRegion = std::make_unique<Region>();
	}

	Tile & tileAt(Coordinates co)
	{
		return currentRegion->tileMap[TILE_ARRAY_LOOKUP];
	}

	Tile & tileAt(int exactPos)
	{
		return currentRegion->tileMap[exactPos];
	}

	bool canWalk(Coordinates co)
	{
		return boundsCheck(co) && (getProperty<Tile::FLOOR>(co) && !(getProperty<Tile::WALL>(co) | getProperty<Tile::OBSTRUCTED>(co)));
	}

	bool boundsCheck(Coordinates co)
	{
		return(co.x < MAP_WIDTH && co.x >= 0 && co.y < MAP_HEIGHT && co.y >= 0 && co.z < MAP_DEPTH && co.z >= 0);
	}

	bool canPass(Coordinates co)
	{
		return boundsCheck(co) && !(getProperty<Tile::WALL>(co) | getProperty<Tile::OBSTRUCTED>(co));
	}

	bool isEmptySpace(Coordinates co)
	{
		return !(getProperty<Tile::FLOOR>(co) | getProperty<Tile::WALL>(co) | getProperty<Tile::OBSTRUCTED>(co));
	}
}


