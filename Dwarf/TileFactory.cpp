#include "TileFactory.h"
#include "Coordinates.h"
#include "Map/Tile.h"

TileFactory * tileFactory;

TileFactory::~TileFactory()
{
}

void TileFactory::createRock(Coordinates co)
{
	tileManager->tileAt(co).ch = 133;
	tileManager->tileAt(co).color = 0xFFD3D3D3;

	// Make sure all rock is mineable
	tileManager->setProperty<Tile::MINEABLE>(co);
}

void TileFactory::createGrass(Coordinates co)
{
	int grassCharList[] = { 130, 147, 244, 244 };
	// Move this to EntityFactory???
	tileManager->tileAt(co).ch = grassCharList[rng.getInt(0, 3, 3)];
	tileManager->tileAt(co).color = 0xC53DFC00;
}

void TileFactory::createRockFloor(Coordinates co)
{
	tileManager->tileAt(co).ch = 44;
	tileManager->tileAt(co).color = 0xAAAFAFAF;

	tileManager->removeProperty<Tile::MINEABLE>(co);
	tileManager->removeProperty<Tile::WALL>(co);
	tileManager->removeProperty<Tile::OBSTRUCTED>(co);
	tileManager->removeProperty<Tile::RAMP>(co);
	tileManager->setProperty<Tile::FLOOR>(co);
}
