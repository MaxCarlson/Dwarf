#pragma once
#include "EntityManager.h"
#include "..\Tile.h"


// POsition components should most likely be set first 
// for almost all entitys that have them.
class PositionComponent : public Component
{
private:
	Coordinates co = { 0, 0, 0 };
public:
	Coordinates coordinates() const { return co; }
	int x() const { return co.x; }
	int y() const { return co.y; }
	int z() const { return co.z; }

	void set(Coordinates cor) { co = cor; }
	void set(int xx, int yy, int zz) { co.x = xx; co.y = yy; co.z = zz; }
};