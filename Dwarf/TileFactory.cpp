#include "TileFactory.h"
#include "Coordinates.h"
#include "Tile.h"

TileFactory::TileFactory(TileManager& tileManager) : tileManager(tileManager)
{
}


TileFactory::~TileFactory()
{
}

void TileFactory::createRock(Coordinates co)
{
	tileManager.tileAt(co).ch = 133;
	tileManager.tileAt(co).color = 0xFFD3D3D3;

	// Make sure all rock is mineable
	tileManager.setProperty<Tile::MINEABLE>(co);
}

void TileFactory::createGrass(Coordinates co)
{
	int grassCharList[] = { 130, 147, 244, 244 };
	// Move this to EntityFactory???
	tileManager.tileAt(co).ch = grassCharList[rng.getInt(0, 3, 3)];
	tileManager.tileAt(co).color = 0xC53DFC00;
}
