#pragma once
#include "include/libtcod.hpp"

struct Tile {
	bool explored;
	// Render map of walls
	Tile() : explored(true) {}
};

class Map
{
public:
	int width, height;

	Map(int width, int height);
	~Map();

	void addMonster(int x, int y);

	bool isWall(int x, int y) const;
	bool canWalk(int x, int y) const;
	bool isInFov(int x, int y) const;
	bool isExplored(int x, int y) const;

	void computeFov();
	void render() const;

protected:
	Tile * tiles;
	TCODMap * map;
	friend class BspListener;


	void dig(int x1, int y1, int x2, int y2);
	void createRoom(bool first, int x1, int y1, int x2, int y2);
};

