#pragma once
#include "include/libtcod.hpp"
#include "Tile.h"

#include <vector>

static const int MAX_ZLVL = 10;

class Tile;

class Map
{
public:
	int width, height, depth;

	Map(int width, int height, int depth);
	~Map();

	void populateRock();

	bool mapIsOkay() const;

	inline void createWall(Coordinates co);
	inline void createWalkableSpace(Coordinates co);
	inline void createOpenSpace(Coordinates co);

	void computeFov();
	void render() const;

	int currentZLevel;
	void incrementZLevel(int inc);
	void jumpToZLevel(int level);

	TileManager tileManager;

protected:

	TCODMap * map;				    // Holds the map the camera will render
	TCODMap * mapZLvls[MAX_ZLVL];   // Holds all of the maps
};


