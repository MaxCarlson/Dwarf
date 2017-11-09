#pragma once
#include "include/libtcod.hpp"
#include "Tile.h"

#include <vector>

// Make this feature dynamic !!
static const int MAX_ZLVL = 10;

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

	bool mapIsOkay() const;

	inline void createWall(Coordinates co);
	inline void createWalkableSpace(Coordinates co);
	inline void createOpenSpace(Coordinates co);

	void render() const;

	// Current Z level of camera
	int currentZLevel;


	bool incrementZLevel(int inc);
	void jumpToZLevel(int level);

	TileManager tileManager;

private:
	TCODRandom * rng;

	TCODMap * map;				    // Holds the map the camera will render
	TCODMap * mapZLvls[MAX_ZLVL];   // Holds all of the maps (each a sepperate z level)
};


