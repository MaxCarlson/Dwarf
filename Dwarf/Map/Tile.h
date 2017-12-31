#pragma once
#include "../Coordinates.h"
#include "Index.h"
#include <vector>
#include <string>

// Formula for tile indexing with coordinates
#define TILE_ARRAY_LOOKUP (co.z * MAP_WIDTH * MAP_HEIGHT) + (co.y * MAP_WIDTH) + co.x

#define CO_NORTH   { co.x,     co.y - 1, co.z }
#define CO_SOUTH   { co.x,     co.y + 1, co.z }
#define CO_EAST    { co.x + 1, co.y,     co.z }
#define CO_WEST    { co.x - 1, co.y,     co.z }
#define CO_NORTH_W { co.x - 1, co.y - 1, co.z }
#define CO_NORTH_E { co.x + 1, co.y - 1, co.z }
#define CO_SOUTH_W { co.x - 1, co.y + 1, co.z }
#define CO_SOUTH_E { co.x + 1, co.y + 1, co.z }
#define CO_UP      { co.x,     co.y,     co.z + 1 }
#define CO_DOWN    { co.x,     co.y,     co.z - 1 }


// These are set inside tile manager when constructing
// Number of tiles for each dimension of the current map
extern int MAP_WIDTH, MAP_HEIGHT, MAP_DEPTH, TOTAL_MAP_TILES;

class Archive;
struct vchar;

namespace region
{

	enum Flag
	{
		CAN_STAND_HERE = 1,
		CAN_GO_NORTH = 1 << 2,
		CAN_GO_SOUTH = 1 << 3,
		CAN_GO_EAST = 1 << 4,
		CAN_GO_WEST = 1 << 5,
		CAN_GO_NORTH_W = 1 << 6,
		CAN_GO_NORTH_E = 1 << 7,
		CAN_GO_SOUTH_E = 1 << 8,
		CAN_GO_SOUTH_W = 1 << 9,
		CAN_GO_UP = 1 << 10,
		CAN_GO_DOWN = 1 << 11,
		CONSTRUCTION = 1 << 12
	};

	enum Directions
	{
		NO_DIRECTION = -1,
		NORTH,
		SOUTH,
		EAST,
		WEST,
		NORTH_W,
		NORTH_E,
		SOUTH_E,
		SOUTH_W,
		UP,
		DOWN
	};

	enum TileTypes
	{
		EMPTY_SPACE,
		SOLID,
		FLOOR,
		WALL,
		RAMP,
	};

	// Create a new region, will overwrite the old region
	void new_region(int width, int height, int depth);
	
	void save_region(std::string filePath);

	void load_region(std::string filePath);

	/* Tile Flag functions, Flags are above */
	bool flag(const Coordinates co, Flag f);

	void setFlag(const Coordinates co, Flag f);

	void resetFlag(const Coordinates co, Flag f);

	bool solid(const int idx);

	//bool aboveGround(const int idx);

	vchar& renderCache(const int idx);

	vchar& getRenderTile(const Coordinates co);

	void setMaterial(const Coordinates co, const std::size_t mat);

	int getTileType(const int idx);

	void setTileType(const int idx, const uint8_t type);

	std::size_t getTileMaterial(const Coordinates co);

	void damageTile(const int idx, const uint16_t dmg);

	uint16_t tileHealth(const int idx);

	std::size_t stockpileId(const int idx);

	void forStockpileSquares(const std::function<void(int, std::size_t)> & func);

	// Recalcuate tile pathing when only one
	// tile has changed, instead of calculating 
	// entire map
	void spot_recalc_paths(const Coordinates co);

	void tile_recalc(const Coordinates co);

	void tile_calc_render(const Coordinates co);

	// Recalculate all pathing Flags on map
	void tile_recalc_all();

	void makeWall(const int idx);

	void makeEarth(const int idx);

	void makeRamp(const int idx);

	void makeFloor(const int idx);

	void makeEmptySpace(const int idx);

	// Get a fast estimate of the distance between two
	// 3D points
	double get_3D_distance(Coordinates loc, Coordinates dest);

	double get_2D_distance(Coordinates loc, Coordinates dest);

	// Checks all bounds in a coordinate to make sure
	// it's entirely valid
	bool boundsCheck(Coordinates co);
}

