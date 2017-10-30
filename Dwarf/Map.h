#pragma once
#include "include/libtcod.hpp"

struct Tile {
	bool explored;
	// Render map of walls
	Tile() : explored(true) {} // MAKE VALUE FALSE TO OCCLUDE unexplored regions
};

static const int MAX_ZLVL = 5;

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
	void changeZLevel(int level);

protected:
	Tile * tiles;
	TCODMap * map;
	TCODMap * mapZLvls[MAX_ZLVL];

	//friend class BspListener;
	//void dig(int x1, int y1, int x2, int y2);
	//void createRoom(bool first, int x1, int y1, int x2, int y2);
};

