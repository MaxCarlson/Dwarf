#pragma once
#include "Coordinates.h"

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
	enum Property
	{
		EXPLORED   = 1,
		OBSTRUCTED = 1 << 1,
		WALL       = 1 << 2,
		FLOOR      = 1 << 3,
		RAMP       = 1 << 4,
		MINEABLE   = 1 << 5
	};

	// Integer representation of tileset index
	// as well as tag used to identify material
	// Will probably use this for matching if tile is mined etc
	int  ch;	

	// Type of material the Tile
	// is made from
	int mat;

	// Use int hex values for colors!!!!
	int color;


	// Add a property here denoting if tile is ore/gem vein for quick lookup!!
	std::uint16_t properties = 0x1; // Change to 0U for unexplored
};

// Formula for tile indexing with coordinates
#define TILE_ARRAY_LOOKUP co.z * width * height + co.y * width + co.x

enum Directions
{
	CAN_GO_NORTH,
	CAN_GO_SOUTH,
	CAN_GO_EAST,
	CAN_GO_WEST,
	CAN_GO_UP,
	CAN_GO_DOWN
};


// Creates a 1D Vector of Tile objects used to
// simulate a 3D area of tiles. Access Tiles through here
class TileManager
{
public:
	// Dimensions of map we're 
	// going to render
	int width, height, depth;

	TileManager(int width, int height, int depth) : width(width), height(height), depth(depth)
	{
		tileMap.resize(width * height * depth);
	}

	// Alters the TileProperty P property of a tile
	// only if that tile did not have TileType T property
	template<Tile::Property P>
	inline void setProperty(Coordinates co)                  // Test all these functions for perf with const Coordinates ?
	{
		tileAt(co).properties |= P;
	}

	// Reverses property TileProperty P of tile
	// Faster than remove if we know what tile prop
	// is ahead of time
	template<Tile::Property P>
	inline void reverseProperty(Coordinates co)
	{
		tileAt(co).properties ^= P;
	}

	// Remove property P from Tile
	template<Tile::Property P>
	inline void removeProperty(Coordinates co)
	{
		tileAt(co).properties &= ~P;
	}

	template<Directions D>
	inline bool canGo(Coordinates co)
	{
		if (D == CAN_GO_NORTH)
			return canWalk({ co.x, co.y - 1, co.z });
		if(D == CAN_GO_SOUTH)
			return canWalk({ co.x, co.y + 1, co.z });
		if (D == CAN_GO_EAST)
			return canWalk({ co.x + 1, co.y, co.z });
		if (D == CAN_GO_WEST)
			return canWalk({ co.x - 1, co.y, co.z });

		// These will need to be changed since enventually 
		// we don't want dwarves walking on air and dropping down 
		if (D == CAN_GO_UP)
			return canWalk({ co.x, co.y, co.z + 1}) && getProperty<Tile::RAMP>(co);
		if (D == CAN_GO_DOWN)
			return canWalk({ co.x, co.y, co.z - 1 }) && getProperty<Tile::RAMP>({co.x, co.y, co.z - 1});

		return false;
	}

	// Returns true if tile has property of TileProperty P
	// For testing multiple properties, OR/XOR two tile properties together
	// Should be named hasProperty
	template<unsigned P>
	const inline bool getProperty(Coordinates co) const
	{
		return tileAt(co).properties & P;
	}

	// Returns a refrence to tile at coordinates
	inline Tile& tileAt(Coordinates co)
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

	// Get the index of Coordinates 
	inline int getIdx(Coordinates co)
	{
		return TILE_ARRAY_LOOKUP;
	}

	// Returns a copy of data at tile below input tile coordinates
	// Worse performance due to use of .at() for safety
	// Is this needed?
	inline Tile tileBelow(Coordinates co) const
	{
		return tileMap.at(TILE_ARRAY_LOOKUP);
	}

	// Checks if it's possible to walk through tile ~~ wall check probably isn't neccasary so long as obstructed is updated correctly!!
	inline bool canWalk(Coordinates co) const
	{
		return boundsCheck(co) && (getProperty<Tile::FLOOR>(co) && !(getProperty<Tile::WALL>(co) | getProperty<Tile::OBSTRUCTED>(co)));
	}

	// Checks to make sure things don't go off the map
	// with bounds checking DOES NOT do z bound checking
	inline bool isOnPlane(Coordinates co) const
	{
		return (co.x < width && co.x >= 0 && co.y < height && co.y >= 0);
	}

	// Checks all bounds in a coordinate to make sure
	// it's entirely valid
	inline bool boundsCheck(Coordinates co) const
	{
		return(co.x < width && co.x >= 0 && co.y < height && co.y >= 0 && co.z < depth && co.z >= 0);
	}

	// Slightly different from canWalk in that it doesn't 
	// check for floor
	inline bool canPass(Coordinates co) const
	{
		return isOnPlane(co) && !(getProperty<Tile::WALL>(co) | getProperty<Tile::OBSTRUCTED>(co)); 
	}

	// Should only be used for map generation, will not gurentee things are completely empty
	// Possibly move this to the map object???????
	inline bool isEmptySpace(Coordinates co) const
	{
		return !(getProperty<Tile::FLOOR>(co) | getProperty<Tile::WALL>(co) | getProperty<Tile::OBSTRUCTED>(co));
	}

private:

	// 1D vector of Tiles indexed by 3D formula
	std::vector<Tile> tileMap;
};
