#pragma once

class TCODRandom;

class VeinCreator
{
public:
	VeinCreator(int mapWidth, int mapHeight, int mapDepth);
		


	~VeinCreator() { delete rng; };

	TCODRandom * rng;

	void calcualteOreDepths();
	void addOre();

private:
	int mapWidth;
	int mapHeight;
	int mapDepth;
};