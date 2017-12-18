#include "Map.h"

#include "../Engine.h"
#include "Tile.h"
#include "MapRender.h"
#include "VeinCreator.h"
#include "../TileFactory.h"
#include "../ECS\Components\PositionComponent.h"
#include "../ECS\Components\RenderComponent.h"
#include "../ECS\Components\HealthComponent.h"
#include "../BearLibTerminal.h"
#include "../Raws/Materials.h"


using namespace region;

static const int MIN_LVLS_OF_ROCK = 40;

Map::Map(int width, int height, int depth) : width(width), height(height), depth(depth)
{
	rng = TCODRandom::getInstance();

	new_region(width, height, depth);
	tileFactory = new TileFactory();

	createHeightMap(7, 0.3f);
	
	populateRock();
	addTrees(10);
	populateGrass();
	seedRamps();
	placeDwarves(7);

	tileRecalcAll();
}

Map::~Map()
{
	delete tileFactory;
	//delete rng;
}
// Needs serious work!!!!
void Map::createHeightMap(int howMountainous, float rainAmount)
{
	// Create height map of area
	// Want to normalize map eventually?
	TCODHeightMap * heightMap = new TCODHeightMap(width, height);
	heightMap->midPointDisplacement(rng);

	//heightMap->normalize(-0.9, 0.9);

	// Add in howMountainous here so we can increase or decrease howMountainous
	double mountinStep = double(depth - MIN_LVLS_OF_ROCK);
	double heightThreshold = 90;


	// If height is below a threshold, mark that area walkable/visible. else not. 
	for (int h = 0; h < depth; ++h) {

		engine->mapRenderer->currentZLevel = h; // Change these functions around when placing player

		for (int i = 0; i < width; ++i)
			for (int j = 0; j < height; ++j)
			{
				// Values between -100 and 100
				double heightMapPoint = heightMap->getValue(i, j) * 100;

				if (h <= MIN_LVLS_OF_ROCK) {															      
					makeWall(getIdx({ i, j, h })); 
				}
				else if (h >= depth - 2 && getProperty<Tile::WALL>({ i, j, h - 1}))
				{
					makeFloor(getIdx({ i, j, h }));
				}
				// If there's a floor but height ratio is too low, create walkable space
				else  if (heightThreshold < heightMapPoint && getProperty<Tile::FLOOR>({ i, j, h }))
				{   
					makeFloor(getIdx({ i, j, h }));
				}
				// If there is a wall below, and height is high enough create more land
				else if (canWalk({ i, j, h }))
				{                                               
					makeWall(getIdx({ i, j, h }));
				}
				// else create open space
				else 
				{														                  
					makeEmptySpace(getIdx({ i, j, h }));
				}

			}
		if (h > MIN_LVLS_OF_ROCK)
		{
			heightThreshold -= mountinStep;
		}
	}
	delete heightMap;

	// Level out max height and make walkable if
	// there are walls at peak 
	// Map is already set to max z level so no need to change it
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
		{
			if (getProperty<Tile::WALL>({ i, j, depth - 1 }))
			{
				makeFloor(getIdx({ i, j, depth - 1 }));
			}
		}

}

void Map::seedRamps()
{
	for (int h = MIN_LVLS_OF_ROCK; h < depth - 1; ++h) 
		for (int i = 0; i < width; ++i)
			for (int j = 0; j < height; ++j)
			{
				if (getProperty<Tile::WALL>({ i, j, h }))
				{
					const Coordinates walkableTop = { i, j, h + 1 };

					const Coordinates northWalk = { i, j + 1, h };
					const Coordinates southWalk = { i, j - 1, h };
					const Coordinates eastWalk  = { i + 1, j, h };
					const Coordinates westWalk  = { i - 1, j, h };

					if (rampSanityCheck(walkableTop, northWalk))
					{
						makeRamp(getIdx(northWalk));
					}
					else if (rampSanityCheck(walkableTop, southWalk))
					{
						makeRamp(getIdx(southWalk));
					}
					else if (rampSanityCheck(walkableTop, eastWalk))
					{
						makeRamp(getIdx(eastWalk));
					}
					else if (rampSanityCheck(walkableTop, westWalk))
					{
						makeRamp(getIdx(westWalk));
					}
				}
			}
	
}

bool Map::rampSanityCheck(Coordinates rampLoc, Coordinates rampLand) const
{
	if (boundsCheck(rampLoc) && canWalk(rampLoc) && boundsCheck(rampLand) && canWalk(rampLand))
		return true;

	return false;
}

// Possibly move all these loops into one or two to minimize looping??
void Map::populateGrass()
{
	int grassCharList[] = { 130, 147, 244, 244 };

	for (int h = MIN_LVLS_OF_ROCK; h < depth; ++h)
		for (int i = 0; i < width; ++i)
			for (int j = 0; j < height; ++j)
			{
				if (    getProperty<Tile::FLOOR>({ i, j, h })
					&& !getProperty<Tile::OBSTRUCTED>({ i, j, h }))
				{
					std::string tmp = "grass";
					int rg = rng->getInt(1, 3, 3);

					tmp += rg + '0';

					//tileFactory->createGrass({ i, j, h });
					region::setMaterial({ i, j, h }, getMaterialIdx(tmp));
				}
			}
}

// Create and use threshold for ore depending on depth
// modify threashold as depth increases to increase/decrease rarity of certain ores
void Map::populateRock()
{
	// Create filler rock
	for (int h = 0; h < depth; ++h)
		for (int i = 0; i < width; ++i)
			for (int j = 0; j < height; ++j)
			{
				if (getProperty<Tile::WALL>({ i, j, h }))
				{
					tileFactory->createRock({ i, j, h });
					region::setMaterial({ i, j, h }, getMaterialIdx("granite_rock"));
				}
			}

	// Add ore veins to map
	//VeinCreator veins(width, height, depth);
	//veins.calcualteOreDepths();
	//veins.addOre(*tileManager);
}

void Map::addTrees(int treeDensity)
{
	// for Bearslib - 5, 6, 7 = bush, 23, 24
	// Use these eventually
	int treeChar = 24;

	int treeNum = ((width * height) / 1000) * treeDensity;

	// Store these these trees in map eventually??!!!!!!!!!!!!!!!!!!
	std::vector<Entity> trees = engine->world.createEntities(treeNum);

	TCODRandom * rng = TCODRandom::getInstance();

	int counter = 0;
	for (Entity &t : trees)
	{
		while (true) {      // Probably a better way to do this than randomly picking points in space!
			Coordinates co;
			co.x = rng->getInt(0, width-1);
			co.y = rng->getInt(0, height-1);
			co.z = rng->getInt(MIN_LVLS_OF_ROCK, depth-1);

			const Tile t = tileAt(co);

			// If the space is clear/hasfloor plant a tree and obstruct the space
			if (t.properties & Tile::FLOOR && !(t.properties & Tile::OBSTRUCTED)) // Move this to EntityFactory???
			{
				trees[counter].addComponent<PositionComponent>(co);
				trees[counter].addComponent<RenderComponent>(treeChar, 2, "brown");
				trees[counter].addComponent<HealthComponent>(300, 300, 0);
				trees[counter].activate();

				setProperty<Tile::OBSTRUCTED>(co);
				break;
			}

		}
		++counter;
	}

}
// Just places dwaves close to where camera is
//
void Map::placeDwarves(int number)
{
	int dwarfNumber = 0;

	for(int h = MIN_LVLS_OF_ROCK; h < depth; ++h)
		for (int i = 3; i < 80; ++i)
			for (int j = 3; j < 80; ++j)
			{
				if (canWalk({ i, j, h }))
				{
					engine->Dwarves.at(dwarfNumber).getComponent<PositionComponent>().co = { i, j, h };
					engine->mapRenderer->currentZLevel = h;
					++dwarfNumber;
				}

				if (dwarfNumber >= number)
				{
					//engine->world.refresh();
					return;
				}
					
			}
}
