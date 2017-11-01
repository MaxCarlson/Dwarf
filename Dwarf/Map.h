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

	//void addMonster(int x, int y);

	inline Tile * tileAt(Coordinates co) const;
	inline Tile * tileBelow(Coordinates co) const;

	bool mapIsOkay()              const;
	bool isWall(Coordinates co)   const;
	bool canWalk(Coordinates co)  const;
	bool isFloor(Coordinates co)  const;
	bool isInFov(int x, int y)    const;
	bool isExplored(int x, int y) const;

	inline void createWall(Coordinates co);
	inline void createWalkableSpace(Coordinates co);
	inline void createOpenSpace(Coordinates co);

	void computeFov();
	void render() const;

	int currentZLevel;
	void incrementZLevel(int inc);
	void jumpToZLevel(int level);

protected:

	Tile    * tiles[MAX_ZLVL];
	TCODMap * map;				    // Holds the map the camera will render
	TCODMap * mapZLvls[MAX_ZLVL];   // Holds all of the maps

	//friend class BspListener;
	//void dig(int x1, int y1, int x2, int y2);
	//void createRoom(bool first, int x1, int y1, int x2, int y2);
};

// Returns a pointer to a tile at coordinates
inline Tile * Map::tileAt(Coordinates co) const
{
	return (tiles[co.z] + (co.x + co.y * height));
}

// Don't ask for tiles below for tiles on z == 1 plane
inline Tile * Map::tileBelow(Coordinates co) const
{
	return co.z > 0 ? (tiles[co.z - 1] + (co.x + co.y * height)) : NULL;
}

