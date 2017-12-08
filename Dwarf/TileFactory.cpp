#include "TileFactory.h"
#include "Coordinates.h"
#include "Map/Tile.h"

using namespace region;

TileFactory * tileFactory;

TileFactory::~TileFactory()
{
}

void TileFactory::createRock(Coordinates co)
{
	tileAt(co).ch = 133;
	tileAt(co).color = 0xFFD3D3D3;

	// Make sure all rock is mineable
	setProperty<Tile::MINEABLE>(co);
}

void TileFactory::createGrass(Coordinates co)
{
	int grassCharList[] = { 130, 147, 244, 244 };
	// Move this to EntityFactory???
	tileAt(co).ch = grassCharList[rng.getInt(0, 3, 3)];
	tileAt(co).color = 0xC53DFC00;
}

void TileFactory::createRockFloor(Coordinates co)
{
	tileAt(co).ch = 44;
	tileAt(co).color = 0xAAAFAFAF;

	removeProperty<Tile::MINEABLE>(co);
	removeProperty<Tile::WALL>(co);
	removeProperty<Tile::OBSTRUCTED>(co);
	removeProperty<Tile::RAMP>(co);
	setProperty<Tile::FLOOR>(co);
}
