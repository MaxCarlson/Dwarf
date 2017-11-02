#pragma once
#include "include\libtcod.hpp"
#include "Entity.h"
#include "Tile.h"

/*
// Holds the map index for the particular componenet
enum CompID
{
	COMPID_RENDER,
	COMPID_PLAYER
};

class Component : public GameObject
{
public:
	Component(CompID compId) : compID(compId) {};
	CompID compID;
};

class RenderComponent : public Component
{
public:
	RenderComponent(CompID compId, TCODColor col, Coordinates cor) : Component(compId), col(col), cor(cor) {};
	TCODColor & col;
	Coordinates & cor;
};

class Input : public Component
{
	Input(CompID compId, TCODColor col, Coordinates cor) : Component(compId), col(col), cor(cor) {};
	TCODColor & col;
	Coordinates & cor;
};
*/

class PositionComponent : public Component
{
private:
	Coordinates co = {0, 0, 0};
public:
	Coordinates coordinates() { return co; }
	int x() { return co.x; }
	int y() { return co.y; }
	int z() { return co.z; }

	void set(Coordinates cor) { co = cor; }
	void set(int xx, int yy, int zz) { co.x = xx; co.y = yy; co.z = zz; }
};