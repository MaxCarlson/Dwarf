#pragma once

#include <vector>


// Holds what it says, used for 
// index lookups
struct Coordinates {
	int x, y, z;
};

class Tile {

public:
	//bool explored;		 // Tile has either started this way, or been exposed and viewed
	//bool obstructed;	 // Tile can't be walked through, applies to character inside tile
	//bool isWall;         
	//bool providesFloor;  // Tile can be walked on, if the Tile on the current map is not obstructed, and the tile directly below providesFloor the tile can be walked on
	//int  ch;			 // In game character repesetation, 0 = invisible

	//Tile() : explored(true), obstructed(true), isWall(true) {} // MAKE exploered FALSE TO OCCLUDE unexplored regions

	std::uint8_t properties;
private:
	
};

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

	// Alters the TileProp T property of a tile
	// only if that tile did not have TileType T property
	template<TileProp P>
	inline void setProperty(Coordinates co)
	{
		tileAt(co).properties | P;
	}

	// Reverses property TileProp T of tile
	template<TileProp P>
	inline void reverseProperty(Coordinates co)
	{
		tileAt(co).properties ^ P;
	}

	// Returns true if tile has property of TileProp T
	template<TileProp P>
	const inline bool getProperty(Coordinates co) const
	{
		return tileAt(co).properties & P;
	}


	inline Tile& tileAt(const Coordinates co)
	{
		return tileMap[co.x + width * (co.y + height * co.z)];
	}

	inline Tile tileAt(const Coordinates co) const
	{
		return tileMap[co.x + width * (co.y + height * co.z)];
	}

	// Returns a copy of data at tile below input tile coordinates
	// Worse performance due to use of .at() for safety
	inline Tile tileBelow(const Coordinates co) const
	{
		return tileMap.at(co.x + width * (co.y + height * (co.z - 1)));
	}

	inline bool canWalk(const Coordinates co) const
	{
		return (getProperty<FLOOR>(co) && !getProperty<WALL>(co) && !getProperty<OBSTRUCTED>(co));
	}

private:
	int width, height, depth;
	std::vector<Tile> tileMap;

	// Used for &'ing against a tiles
	// compressed int properties to attain
	// a value / set a value
	static const int explored = 0x1;
	static const int obstructed = 0x2;
	static const int wall = 0x4;
	static const int floor = 0x8;
};

