#pragma once


// Holds what it says, used for 
// index lookups
struct Coordinates {
	int x, y, z;
};


class Tile {

public:
	bool explored;		 // Tile has either started this way, or been exposed and viewed
	bool obstructed;	 // Tile can't be walked through, applies to character inside tile
	bool isWall;         
	bool providesFloor;  // Tile can be walked on, if the Tile on the current map is not obstructed, and the tile directly below providesFloor the tile can be walked on
	int  ch;			 // In game character repesetation, 0 = invisible

	Tile() : explored(true), obstructed(true), isWall(true), ch(0) {} // MAKE exploered FALSE TO OCCLUDE unexplored regions

private:

};

