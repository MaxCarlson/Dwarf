#pragma once
#include "Coordinates.h"

#include <vector>





class Tile {

public:
	// These properties are set by set bit's in Tile's properties unsigned int
	// And accessed through the tile manager
	// bool explored;		 
	// bool obstructed;	 
	// bool isWall;         
	// bool providesFloor;  

	// Integer representation of tileset index
	// as well as tag used to identify material
	// Will probably use this for matching if tile is mined etc
	int  ch;	

	// Use int hex values for colors!!!!
	std::string color;


	// Add a property here denoting if tile is ore/gem vein for quick lookup!!
	std::uint8_t properties = 0x1U; // Change to 0U for unexplored
private:
	
};

// Formula for tile indexing with coordinates
#define TILE_ARRAY_LOOKUP co.z * width * height + co.y * width + co.x

// Creates a 1D Vector of Tile objects used to
// simulate a 3D area of tiles. Access Tiles through here
class TileManager
{
public:
	TileManager(int width, int height, int depth) : width(width), height(height), depth(depth)
	{
		tileMap.resize(width * height * depth);
		tileMapSize = tileMap.size();
	}

	enum TileProp
	{
		EXPLORED = 0x1,
		OBSTRUCTED = 0x2,
		WALL = 0x4,
		FLOOR = 0x8
	};

	// Alters the TileProperty P property of a tile
	// only if that tile did not have TileType T property
	template<TileProp P>
	inline void setProperty(Coordinates co)                  // Test all these functions for perf with const Coordinates ?
	{
		tileAt(co).properties |= P;
	}

	// Reverses property TileProperty P of tile
	// Faster than remove if we know what tile prop
	// is ahead of time
	template<TileProp P>
	inline void reverseProperty(Coordinates co)
	{
		tileAt(co).properties ^= P;
	}

	// Remove property P from Tile
	template<TileProp P>
	inline void removeProperty(Coordinates co)
	{
		tileAt(co).properties &= ~P;
	}

	// Returns true if tile has property of TileProperty P
	template<TileProp P>
	const inline bool getProperty(Coordinates co) const
	{
		return tileAt(co).properties & P;
	}

	// Returns a refrence to tile at coordinates
	inline Tile& tileAt(const Coordinates co)
	{
		return tileMap[TILE_ARRAY_LOOKUP];
	}

	// Returns a copy of tile at coordinates
	inline Tile tileAt(Coordinates co) const
	{
		return tileMap[TILE_ARRAY_LOOKUP];
	}

	// Direct lookup by tile index instead of coordinates
	inline Tile tileAt(int exactPos) const
	{
		return tileMap[exactPos];
	}

	// Returns a copy of data at tile below input tile coordinates
	// Worse performance due to use of .at() for safety
	inline Tile tileBelow(Coordinates co) const
	{
		return tileMap.at(TILE_ARRAY_LOOKUP);
	}

	// Checks if it's possible to walk through tile ~~ wall check probably isn't neccasary so long as obstructed is updated correctly!!
	inline bool canWalk(Coordinates co) const
	{
		return isSafe(co) && (getProperty<FLOOR>(co) && !(getProperty<WALL>(co) | getProperty<OBSTRUCTED>(co)));
	}

	// Used for fast bounds checking on coordinates
	// Doesn't work for checking map edge boundries,
	// only validity of index
	inline bool isSafe(Coordinates co) const
	{
		const int cor = TILE_ARRAY_LOOKUP;
		return (cor > 0 && cor < tileMapSize);
	}

	// Checks to make sure things don't go off the map
	// with bounds checking
	inline bool isOnPlane(Coordinates co) const
	{
		return (co.x < width && co.x >= 0 && co.y < height && co.y >= 0);
	}

	// Slightly different from canWalk in that it doesn't 
	// check for floor
	inline bool canPass(Coordinates co) const
	{
		return isOnPlane(co) && !(getProperty<WALL>(co) | getProperty<OBSTRUCTED>(co)); //isSafe(co)
	}

	// Should only be used for map generation, will not gurentee things are completely empty
	// Possibly move this to the map object???????
	inline bool isEmptySpace(Coordinates co) const
	{
		return !(getProperty<FLOOR>(co) | getProperty<WALL>(co) | getProperty<OBSTRUCTED>(co));
	}

	// enum for template function
	// describing adjacent tiles ~ partially in use
	enum TileAdj
	{
		NORTH,
		NORTH_UP,
		NORTH_DOWN,
		SOUTH,
		SOUTH_UP,
		SOUTH_DOWN,
		EAST,
		EAST_UP,
		EAST_DOWN,
		WEST,
		WEST_UP,
		WEST_DOWN
	};


private:
	// Dimensions of map we're 
	// going to render
	int width, height, depth;

	// 1D vector of Tiles indexed by 3D formula
	std::vector<Tile> tileMap;

	// Holds the size of the tileMap 
	// so we can quickly check if a position
	// is in bounds with > -1 < tileMapSize
	int tileMapSize;
};
