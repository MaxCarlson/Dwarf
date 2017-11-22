#include "VeinCreator.h"

#include "Engine.h"
#include "Map.h"
#include "Tile.h"
#include <string>

static const int coalType  = 136;
static const int oreType   = 137;
static const int smallOre  = 138;
static const int mediumOre = 139;

struct OreType
{
	OreType(int ch, std::string color) : ch(ch), color(color) {};
	int ch;
	std::string color;
};

enum OreIndex
{
	IRON,
	COPPER,
	TIN,
	COAL
};

static const int numOreTypes = 4;

#define iron   OreType(139, "#434b4d")
#define copper OreType(139, "#B87333")
#define tin    OreType(139, "#C0C0C0")
#define coal   OreType(136, "#5C5B5D")

OreType oreArray[numOreTypes] = { iron, copper, tin, coal };
int oreDepths[numOreTypes]                 = {    0,      0,   0,    0 };
float medianOreDepths[numOreTypes]         = { 0.35,   0.55,0.75, 0.75 }; // In % of map z level

int oreLowerBound[numOreTypes] = { 0, 0, 0, 0 };
int oreUpperBound[numOreTypes] = { 0, 0, 0, 0 };

VeinCreator::VeinCreator(int mapWidth, int mapHeight, int mapDepth) 
	       : mapWidth(mapWidth), mapHeight(mapHeight), mapDepth(mapDepth)
{
	rng = new TCODRandom();

	oreArray[IRON] = iron;
	oreArray[COPPER] = copper;
	oreArray[TIN] = tin;
	oreArray[COAL] = coal;
}

void VeinCreator::calcualteOreDepths()
{
	// Iron ore depth calc
	oreUpperBound[IRON] = rng->getInt(mapDepth * 0.85, mapDepth, medianOreDepths[IRON] * mapDepth) + 5;
	oreLowerBound[IRON] = 0;

	oreUpperBound[COPPER] = rng->getInt(oreUpperBound[IRON] - 5, mapDepth, medianOreDepths[COPPER] * mapDepth) + 5;
	oreLowerBound[COPPER] = rng->getInt(5, 15);

	oreUpperBound[TIN] = rng->getInt(oreUpperBound[COPPER] - 5, mapDepth, medianOreDepths[TIN] * mapDepth) + 5;
	oreLowerBound[TIN] = rng->getInt(10, 20);

	oreUpperBound[COAL] = rng->getInt(oreUpperBound[TIN] - 5, mapDepth, medianOreDepths[COAL] * mapDepth) + 5;
	oreLowerBound[COAL] = rng->getInt(15, 30);
}

static const int oreFrequencyPlvl[] = {10, 15, 15, 9};
static const int orePerVein[] = { 45, 75, 75, 66 };

// This needs serious work!!!
void VeinCreator::addOre(TileManager & tileManager)
{
	for(int ore = 0; ore < numOreTypes; ++ore)
		for (int h = 1; h < mapDepth; ++h)
		{
			if (h < oreLowerBound[ore])
				h = oreLowerBound[ore];

			if (h > oreUpperBound[ore])
				break;

			for (int i = 0; i < oreFrequencyPlvl[ore]; ++i)
			{
				Coordinates co;
				co.z = h;
				co.x = rng->getInt(0, mapWidth - 1);
				co.y = rng->getInt(0, mapHeight - 1);

				if (tileManager.getProperty<Tile::WALL>(co))
				{
					for (int j = 0; j < orePerVein[ore]; ++j)
					{
						if (tileManager.getProperty<Tile::WALL>(co))
						{
							tileManager.tileAt(co).ch = oreArray[ore].ch;
							tileManager.tileAt(co).color = oreArray[ore].color;
						}

						int r = rng->getInt(0, 2);

						switch (r)
						{
						case 0:
							co.x += rng->getInt(-1, 1);
							break;

						case 1:
							co.y += rng->getInt(-1, 1);
							break;

						case 2:
							co.x += rng->getInt(-1, 1);
							co.y += rng->getInt(-1, 1);
							break;
						}
					}
				}
			}
		}
}
