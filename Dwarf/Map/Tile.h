#pragma once
#include "../Coordinates.h"

#include <vector>

class Tile {

public:
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

	// Type of material the Tile
	// is made from
	int mat;

	// Use int hex values for colors!!!!
	int color;

	int health = 500;

	// Add a property here denoting if tile is ore/gem vein for quick lookup!!
	std::uint16_t properties = 0x1; // Change to 0U for unexplored
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

	// Get the index of Coordinates 
	// Returns 0 if index isn't in the map
	int getIdx(Coordinates co);


	// Returns Coordinates from a vector index
	Coordinates idxToCo(int idx);

	bool flag(Coordinates co, Flag f);

	//void setFlag();

	//void resetFlag();

	void tileRecalcAll();


	void makeWall(const int idx);

	void makeRamp(const int idx);

	void makeFloor(const int idx);

	void makeEmptySpace(const int idx);


	int get_rough_distance(Coordinates loc, Coordinates dest);


	void new_region(int width, int height, int depth);


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

	template<Flag D> // Wil be deleting this and replacing with flags
	bool canGo(Coordinates co)
	{
		if (D == CAN_GO_NORTH)
			return canWalk({ co.x, co.y - 1, co.z });
		if (D == CAN_GO_SOUTH)
			return canWalk({ co.x, co.y + 1, co.z });
		if (D == CAN_GO_EAST)
			return canWalk({ co.x + 1, co.y, co.z });
		if (D == CAN_GO_WEST)
			return canWalk({ co.x - 1, co.y, co.z });

		if (D == CAN_GO_NORTH_W)
			return  canWalk({ co.x - 1, co.y - 1, co.z });

		if (D == CAN_GO_NORTH_E)
			return  canWalk({ co.x + 1, co.y - 1, co.z });

		if (D == CAN_GO_SOUTH_E)
			return canWalk({ co.x + 1, co.y + 1, co.z });

		if (D == CAN_GO_SOUTH_W)
			return canWalk({ co.x - 1, co.y + 1, co.z });

		// These will need to be changed since enventually 
		// we don't want dwarves walking on air and dropping down 
		if (D == CAN_GO_UP)
			return canWalk({ co.x, co.y, co.z + 1 }) && getProperty<Tile::RAMP>(co);
		if (D == CAN_GO_DOWN)
			return canWalk({ co.x, co.y, co.z - 1 }) && getProperty<Tile::RAMP>({ co.x, co.y, co.z - 1 });

		return false;
	}

	template<Directions D>
	Coordinates positionAt(Coordinates co) // Should probably delete this an replace with a define?
	{
		if (co.z >= MAP_DEPTH || co.z < 0 || co.y >= MAP_HEIGHT || co.y < 0 || co.x >= MAP_WIDTH || co.x < 0)
			return { 0, 0, 0 };

		if (D == NORTH)
			return { co.x, co.y - 1, co.z };

		if (D == SOUTH)
			return{ co.x, co.y + 1, co.z };

		if (D == EAST)
			return { co.x + 1, co.y, co.z };

		if (D == WEST)
			return { co.x - 1, co.y, co.z };

		if (D == NORTH_W)
			return { co.x - 1, co.y - 1, co.z };

		if (D == NORTH_E)
			return { co.x + 1, co.y - 1, co.z };

		if (D == SOUTH_E)
			return { co.x + 1, co.y + 1, co.z };

		if (D == SOUTH_W)
			return { co.x - 1, co.y + 1, co.z };

		if (D == UP)
			return { co.x, co.y, co.z + 1 };

		if (D == DOWN)
			return { co.x, co.y, co.z - 1 };

		return { 0, 0, 0 };
	}
}

