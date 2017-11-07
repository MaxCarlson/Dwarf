#pragma once

#include <vector>


// Holds what it says, used for 
// index lookups
struct Coordinates {
	int x, y, z;
};


class Tile {

public:
	// These properties are set by set bit's in Tile's properties unsigned int
	// And accessed through the tile manager
	// bool explored;		 
	// bool obstructed;	 
	// bool isWall;         
	// bool providesFloor;  


	//int  ch;			

	std::uint8_t properties = 0L;
private:
	
};

// Creates a 1D Vector of Tile objects used to
// simulate a 3D area of tiles. Access Tiles through here
class TileManager
{
public:
	TileManager(int width, int height, int depth) : width(width), height(height), depth(depth)
	{
		tileMap.resize(width * height * depth);
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
	template<TileProp P>
	inline void reverseProperty(Coordinates co)
	{
		tileAt(co).properties ^= P;
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
		return tileMap[co.z * width * height + co.y * width + co.x];
	}

	// Returns a copy of tile at coordinates
	inline Tile tileAt(Coordinates co) const
	{
		return tileMap[co.z * width * height + co.y * width + co.x];
	}

	// Returns a copy of data at tile below input tile coordinates
	// Worse performance due to use of .at() for safety
	inline Tile tileBelow(Coordinates co) const
	{
		return tileMap.at(co.z * width * height + co.y * width + co.x);
	}

	// Checks if it's possible to walk through tile
	inline bool canWalk(Coordinates co) const
	{
		return (getProperty<FLOOR>(co) && !getProperty<WALL>(co) && !getProperty<OBSTRUCTED>(co));
	}

private:
	// Dimensions of map we're creating
	int width, height, depth;

	// 1D vector of Tiles indexed by 3D formula
	std::vector<Tile> tileMap;
};

