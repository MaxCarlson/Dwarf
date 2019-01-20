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
		CAN_STAND_HERE			= 1,
		CAN_GO_NORTH			= 1 << 1, 
		CAN_GO_SOUTH			= 1 << 2,
		CAN_GO_EAST				= 1 << 3,
		CAN_GO_WEST				= 1 << 4,
		CAN_GO_NORTH_W			= 1 << 5,
		CAN_GO_NORTH_E			= 1 << 6,
		CAN_GO_SOUTH_E			= 1 << 7,
		CAN_GO_SOUTH_W			= 1 << 8,
		CAN_GO_UP				= 1 << 9,
		CAN_GO_DOWN				= 1 << 10,
		CAN_GO_ALL_DIRECTIONS	= ((CAN_GO_DOWN) - 1) & ~1,
		CONSTRUCTION			= 1 << 12,
		DISCOVERED				= 1 << 13
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
		UP_STAIRS,
		DOWN_STAIRS,
		UP_DOWN_STAIRS,
		TREE_TRUNK,
		TREE_LEAF
	};

	// Create a new region, will overwrite the old region
	void new_region(int width, int height, int depth, size_t biomeIdx);
	
	void save_region(std::string filePath);

	void load_region(std::string filePath);

// Tile Flag functions, Flags are above 

	bool flag(const Coordinates co, Flag f);

	void setFlag(const Coordinates co, Flag f);

	void resetFlag(const Coordinates co, Flag f);

	void setSolid(const int idx);

	bool solid(const int idx);

	//bool aboveGround(const int idx);

// Misc tile functions

	vchar& renderCache(const int idx);

	std::pair<vchar, int> getRenderTile(const Coordinates co);

	void setMaterial(const Coordinates co, const std::size_t mat);

	int getTileType(const int idx);

	void setTileType(const int idx, const uint8_t type);

	std::size_t getTileMaterial(const Coordinates co);

	void damageTile(const int idx, const uint16_t dmg);

	uint16_t tileHealth(const int idx);


// Stockpile stuff
	void setStockpileId(const int idx, const std::size_t id);

	std::size_t stockpileId(const int idx);

	void forStockpileSquares(const std::function<void(int, std::size_t)> & func);

// Tree stuff
	int nextTreeId();

	void setTreeId(const int idx, const int id);

	int treeId(const int idx);

	void setTreeHealth(const int id, const int hp);

	int treeHealth(const int id);

	void damageTree(const int id, const int dmg);

	void deleteTree(const int id);

// Plant stuff

	size_t plantType(const int idx);

	uint8_t plantHealth(const int idx);

	uint16_t plantTicker(const int idx);

	uint8_t plantLifeCycle(const int idx);

	void setPlantType(const int idx, const uint8_t type);

	void setPlantHealth(const int idx, const uint8_t hp);

	void setPlantTicker(const int idx, const uint16_t tick);

	void setPlantLifecycle(const int idx, const uint8_t lc);

	void damagePlant(const int idx, const int dmg);

	void setFarmPlot(const int idx);

	void deleteFarmPlot(const int idx);

	void forFarmPlots(std::function<void(const int)> func);

// Pathing and tile calcuating 

	// Recalcuate tile pathing when only one
	// tile has changed, instead of calculating 
	// entire map
	void spot_recalc_paths(const Coordinates co);

	void tile_recalc(const Coordinates co);

	void tile_calc_render(const Coordinates co);

	// Recalculate all pathing Flags on map
	void tile_recalc_all();

// Change tile to type
	void makeWall(const int idx);

	void makeEarth(const int idx);

	void makeRamp(const int idx);

	void makeUpStair(const int idx);

	void makeDownStair(const int idx);

	void makeUpDownStair(const int idx);

	void makeFloor(const int idx);

	void makeEmptySpace(const int idx);

	int groundLevel(const int x, const int y);

// Move these to indices?

	// Checks all bounds in a coordinate to make sure
	// it's entirely valid
	bool boundsCheck(Coordinates co);
}

inline double get_2D_distance(const Coordinates loc, const Coordinates dest)
{
	const double x = static_cast<double>(loc.x - dest.x);
	const double y = static_cast<double>(loc.y - dest.y);

	return std::sqrt((x * x) + (y * y));
}

// Get a fast estimate of the distance between two 2D points
inline double fast_2D_distance(const Coordinates loc, const Coordinates dest)
{
	const double x = static_cast<double>(loc.x - dest.x);
	const double y = static_cast<double>(loc.y - dest.y);

	return (x * x) + (y * y);
}

inline double get_3D_distance(const Coordinates loc, const Coordinates dest)
{
	const double x = static_cast<double>(loc.x - dest.x);
	const double y = static_cast<double>(loc.y - dest.y);
	const double z = static_cast<double>(loc.z - dest.z);

	return std::sqrt((x * x) + (y * y) + (z * z));
}

// Get a fast estimate of the distance between two 3D points
inline double fast_3D_distance(const Coordinates loc, const Coordinates dest)
{
	const double x = static_cast<double>(loc.x - dest.x);
	const double y = static_cast<double>(loc.y - dest.y);
	const double z = static_cast<double>(loc.z - dest.z);

	return (x * x) + (y * y) + (z * z);
}



