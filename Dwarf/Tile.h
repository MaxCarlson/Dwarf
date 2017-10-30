#pragma once






class Tile {

public:
	bool explored;
	bool walkable;
	int ch;			// 0 = invisible
	// Render map of walls
	Tile() : explored(true), walkable(false), ch(0) {} // MAKE VALUE FALSE TO OCCLUDE unexplored regions

private:

};

