#pragma once
#include "include/libtcod.hpp"
#include "Tile.h"

#include <vector>

// Make this feature dynamic !!
static const int MAX_ZLVL = 50;

class MapRender;

class Map
{
public:
	int width, height, depth;

	Map(int width, int height, int depth);
	~Map();

	void createHeightMap(int howMountainous, float rainAmount);
	void seedRamps();
	void populateGrass();
	void populateRock();

	// Takes a number, 0 - 1000
	// representing approx % / 10 of land covered by trees
	void addTrees(int treeDensity);

	void placeDwarves(int number);

	inline void createWall(Coordinates co);
	inline void createWalkableSpace(Coordinates co);
	inline void createOpenSpace(Coordinates co);

	// Performs the rendering of the map
	// and associated built with map generation tiles
	// Should these be moved to Entities, and an array setup for which to render depending on z level?
	MapRender * mapRenderer;

	// Access and alter terrain tiles
	// through this object
	TileManager tileManager;

private:
	TCODRandom * rng;

	TCODMap * mapZLvls[MAX_ZLVL];   // Holds all of the maps (each a sepperate z level)

	friend class MapRender;
};


