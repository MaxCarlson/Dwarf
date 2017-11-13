#pragma once

class TCODRandom;
class TileManager;

class VeinCreator
{
public:
	VeinCreator(int mapWidth, int mapHeight, int mapDepth);
		


	~VeinCreator() { delete rng; };

	TCODRandom * rng;

	void calcualteOreDepths();
	void addOre(TileManager & tileManager);

private:
	int mapWidth;
	int mapHeight;
	int mapDepth;
};