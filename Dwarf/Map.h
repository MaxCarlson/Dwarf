#pragma once
#include "include/libtcod.hpp"


static const int MAX_ZLVL = 10;

class Tile;

class Map
{
public:
	int width, height, depth;

	Map(int width, int height, int depth);
	~Map();

	//void addMonster(int x, int y);

	bool mapIsOkay() const;
	bool isWall(int x, int y) const;
	bool canWalk(int x, int y) const;
	bool isInFov(int x, int y) const;
	bool isExplored(int x, int y) const;

	void computeFov();
	void render() const;

	int currentZLevel;
	void incrementZLevel(int inc);
	void jumpToZLevel(int level);

protected:
	Tile    * tiles;				// Tiles for current z level
	Tile    * tilesZLvls[MAX_ZLVL]; // All tiles
	TCODMap * map;				    // Holds the map the camera will render
	TCODMap * mapZLvls[MAX_ZLVL];   // Holds all of the maps

	//friend class BspListener;
	//void dig(int x1, int y1, int x2, int y2);
	//void createRoom(bool first, int x1, int y1, int x2, int y2);
};

