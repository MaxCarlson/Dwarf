#pragma once
#include "../Coordinates.h"
#include "Index.h"
#include <vector>

class Tile {

public:
	Tile() = default;
	// These properties are set by set bit's in Tile's properties unsigned int
	// And accessed through the tile manager getProperty, setProperty, reverseProperty
	// bool Explored;		 
	// bool Obstructed;	 
	// bool Wall;         
	// bool Floor;  
	// bool Stairs/Ramp ~ Provides z level access changes
	// bool Mineable
	enum Property // In future remove some properties from here and place them into sepperate vectors!!
	{
		EXPLORED = 1,
		OBSTRUCTED = 1 << 1,
		WALL = 1 << 2,
		FLOOR = 1 << 3,
		RAMP = 1 << 4,
		MINEABLE = 1 << 5
	};

	// Integer representation of tileset index
	// as well as tag used to identify material
	// Will probably use this for matching if tile is mined etc
	int ch;

	// Use int hex values for colors!!!!
	int color;

	int health = 500;

	// Add a property here denoting if tile is ore/gem vein for quick lookup!!
	std::uint16_t properties = 0x1; // Change to 0U for unexplored

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(ch, color, health, properties);
	}
};

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
		CAN_GO_DOWN = 1 << 11
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

	// Create a new region, will overwrite the old region
	void new_region(int width, int height, int depth);

	void save_region();

	void load_region();

	/* Tile Flag functions, Flags are above */
	bool flag(const Coordinates co, Flag f);

	void setFlag(const Coordinates co, Flag f);

	void resetFlag(const Coordinates co, Flag f);

	// Recalcuate tile pathing when only one
	// tile has changed, instead of calculating 
	// entire map
	void spot_recalc_paths(const Coordinates co);

	//void tileRecalc(const Coordinates co);

	// Recalculate all pathing Flags on map
	void tileRecalcAll();

	void makeWall(const int idx);

	void makeRamp(const int idx);

	void makeFloor(const int idx);

	void makeEmptySpace(const int idx);

	// Get a fast estimate of the distance between two
	// 3D points
	int get_rough_distance(Coordinates loc, Coordinates dest);

	// Returns a refrence to tile at coordinates
	Tile& tileAt(Coordinates co);


	Tile& tileAt(int exactPos);


	// Checks if it's possible to walk through tile ~~ wall check probably isn't neccasary so long as obstructed is updated correctly!!
	bool canWalk(Coordinates co);


	// Checks all bounds in a coordinate to make sure
	// it's entirely valid
	bool boundsCheck(Coordinates co);


	// Slightly different from canWalk in that it doesn't 
	// check for floor
	bool canPass(Coordinates co);


	// Should only be used for map generation, will not gurentee things are completely empty
	// Possibly move this to the map object???????
	bool isEmptySpace(Coordinates co);

	// Should be called hasProperty
	template<unsigned P>
	const bool getProperty(Coordinates co)
	{
		return tileAt(co).properties & P;
	}

	// Alters the TileProperty P property of a tile
	// only if that tile did not have TileType T property
	template<Tile::Property P>
	void setProperty(Coordinates co)
	{
		tileAt(co).properties |= P;
	}

	// Reverses property TileProperty P of tile
	// Faster than remove if we know what tile prop
	// is ahead of time
	template<Tile::Property P>
	void reverseProperty(Coordinates co)
	{
		tileAt(co).properties ^= P;
	}

	// Remove property P from Tile
	template<Tile::Property P>
	void removeProperty(Coordinates co)
	{
		tileAt(co).properties &= ~P;
	}


}

