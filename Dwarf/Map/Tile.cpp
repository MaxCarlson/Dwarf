#include "Tile.h"
#include "Util.h"
#include <memory>
#include <iostream>

// External map info
int MAP_WIDTH, MAP_HEIGHT, MAP_DEPTH, TOTAL_MAP_TILES;

#define BOUNDS_CHECKING

namespace region
{
	// Creates a 1D Vector of Tile objects used to
	// simulate a 3D area of tiles. Access Tiles through here
	struct Region
	{
		Region()
		{
			tileMap.resize(TOTAL_MAP_TILES);
			tileFlags.resize(TOTAL_MAP_TILES);
		}

		// 1D vector of Tiles indexed by 3D formula
		std::vector<Tile> tileMap;
		std::vector<Util::Bitset<Util::BITSET_16>> tileFlags;

		void tileRecalcAll();
		void tilePathing(Coordinates co);
	};


	std::unique_ptr<Region> currentRegion;


	int getIdx(Coordinates co)
	{
	#ifdef BOUNDS_CHECKING
	
		if (co.z >= MAP_DEPTH || co.z < 0 || co.y >= MAP_HEIGHT || co.y < 0 || co.x >= MAP_WIDTH || co.x < 0)
			std::cout << "Map Idx Bounds Fail!!!" << std::endl;
		
	#endif // BOUNDS_CHECKING
	
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

	bool flag(Coordinates co, Flag f)
	{
		return currentRegion->tileFlags[getIdx(co)].test(f);
	}

	void tileRecalcAll()
	{
		currentRegion->tileRecalcAll();
	}

	void makeWall(const int idx)
	{
		currentRegion->tileFlags[idx].reset(CAN_STAND_HERE);
		setProperty<Tile::OBSTRUCTED>(idxToCo(idx));
		setProperty<Tile::WALL>(idxToCo(idx));

		const Coordinates co = idxToCo(idx);
		// Add a floor property
		// to any Tile above a wall Tile DELETE THIS EVENTUALLY
		if (co.z < MAP_DEPTH - 1)
			setProperty<Tile::FLOOR>({ co.x, co.y, co.z + 1 });
	}

	void makeRamp(const int idx)
	{
		const Coordinates co = idxToCo(idx);
		currentRegion->tileFlags[idx].set(CAN_STAND_HERE);

		tileAt(co).ch = 30;
		setProperty<Tile::RAMP>(co);
		setProperty<Tile::FLOOR>({ co.x, co.y, co.z + 1 });
	}

	void makeFloor(const int idx)
	{
		currentRegion->tileFlags[idx].set(CAN_STAND_HERE);

		setProperty<Tile::FLOOR>(idxToCo(idx));
		removeProperty<Tile::OBSTRUCTED>(idxToCo(idx));
		removeProperty<Tile::WALL>(idxToCo(idx));
	}

	void makeEmptySpace(const int idx)
	{
		currentRegion->tileFlags[idx].reset(CAN_STAND_HERE);
		
		removeProperty<Tile::OBSTRUCTED>(idxToCo(idx));
		removeProperty<Tile::WALL>(idxToCo(idx));
		removeProperty<Tile::FLOOR>(idxToCo(idx));
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

	void Region::tileRecalcAll()
	{
		for (int z = 1; z < MAP_DEPTH - 1; ++z)
			for (int y = 0; y < MAP_HEIGHT - 1; ++y)
				for (int x = 0; x < MAP_WIDTH - 1; ++x)
					tilePathing({ x, y, z });
	}

	void Region::tilePathing(Coordinates co)
	{
		const auto idx = getIdx(co);

		tileFlags[idx].reset(CAN_GO_NORTH);
		tileFlags[idx].reset(CAN_GO_SOUTH);
		tileFlags[idx].reset(CAN_GO_EAST);
		tileFlags[idx].reset(CAN_GO_WEST);
		tileFlags[idx].reset(CAN_GO_NORTH_W);
		tileFlags[idx].reset(CAN_GO_NORTH_E);
		tileFlags[idx].reset(CAN_GO_SOUTH_W);
		tileFlags[idx].reset(CAN_GO_SOUTH_E);
		tileFlags[idx].reset(CAN_GO_DOWN);
		tileFlags[idx].reset(CAN_GO_UP);

		if (getProperty<Tile::WALL>(co))
		{
		}
		else
		{
			if (co.y > 0 && tileFlags[getIdx(CO_NORTH)].test(CAN_STAND_HERE)) { 
				tileFlags[idx].set(CAN_GO_NORTH);
			}

			if (co.y < MAP_HEIGHT - 1 && tileFlags[getIdx(CO_SOUTH)].test(CAN_STAND_HERE)) tileFlags[idx].set(CAN_GO_SOUTH);

			if (co.x > 0             && tileFlags[getIdx(CO_WEST)].test(CAN_STAND_HERE)) tileFlags[idx].set(CAN_GO_WEST);
			if (co.x < MAP_WIDTH - 1 && tileFlags[getIdx(CO_EAST)].test(CAN_STAND_HERE)) tileFlags[idx].set(CAN_GO_EAST);

			if (co.y > 0 && co.x > 0             && tileFlags[getIdx(CO_NORTH_W)].test(CAN_STAND_HERE)) tileFlags[idx].set(CAN_GO_NORTH_W);
			if (co.y > 0 && co.x < MAP_WIDTH - 1 && tileFlags[getIdx(CO_NORTH_E)].test(CAN_STAND_HERE)) tileFlags[idx].set(CAN_GO_NORTH_E);

			if (co.y < MAP_HEIGHT - 1 && co.x > 0             && tileFlags[getIdx(CO_SOUTH_W)].test(CAN_STAND_HERE)) tileFlags[idx].set(CAN_GO_SOUTH_W);
			if (co.y < MAP_HEIGHT - 1 && co.x < MAP_WIDTH - 1 && tileFlags[getIdx(CO_SOUTH_E)].test(CAN_STAND_HERE)) tileFlags[idx].set(CAN_GO_SOUTH_E);

			if (co.z < MAP_DEPTH - 1 && tileFlags[getIdx(CO_UP)].test(CAN_STAND_HERE)   && getProperty<Tile::RAMP>(co     )) tileFlags[idx].set(CAN_GO_UP);
			if (co.z > 0             && tileFlags[getIdx(CO_DOWN)].test(CAN_STAND_HERE) && getProperty<Tile::RAMP>(CO_DOWN)) tileFlags[idx].set(CAN_GO_DOWN);		
		}
	}
}


