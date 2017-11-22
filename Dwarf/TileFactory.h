#pragma once

#include <libtcod.hpp>
class TileManager;
struct Coordinates;

// A place to simplify the creation
// of all types of tiles as well as
// the lookup of tileTypes
class TileFactory
{
public:
	TileFactory(TileManager& tileManager);
	~TileFactory();


	// These functions need to be expanded dramatically.
	// Ore creations should probably be moved here, but the ore gen code
	// can stay in VeinCreator
	void createRock(Coordinates co);

	void createGrass(Coordinates co);

private:
	TileManager& tileManager;
	TCODRandom rng;
};

