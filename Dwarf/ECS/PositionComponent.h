#pragma once
#include "EntityManager.h"
#include "..\Tile.h"



class PositionComponent : public Component
{
private:
	Coordinates co = { 0, 0, 0 };
public:
	Coordinates coordinates() { return co; }
	int x() { return co.x; }
	int y() { return co.y; }
	int z() { return co.z; }

	void set(Coordinates cor) { co = cor; }
	void set(int xx, int yy, int zz) { co.x = xx; co.y = yy; co.z = zz; }
};