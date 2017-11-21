#include "Map.h"

#include "Engine.h"
#include "Tile.h"
#include "MapRender.h"
#include "VeinCreator.h"
#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\Components\HealthComponent.h"

#include "BearLibTerminal.h"

#include "ECS\Systems\MovementAiSystem.h"

static const int MIN_LVLS_OF_ROCK = 40;

Map::Map(int width, int height, int depth) : width(width), height(height), depth(depth), tileManager(width, height, depth)
{
	rng = TCODRandom::getInstance();

	mapRenderer = new MapRender(*this);

	for (int i = 0; i < MAX_ZLVL; ++i) {
		mapZLvls[i] = new TCODMap(width, height);
	}

	createHeightMap(3, 0.3f);
	
	populateRock();
	addTrees(10);
	populateGrass();
	seedRamps();
	placeDwarves(7);
}

Map::~Map()
{
	for (int i = 0; i < MAX_ZLVL; ++i)
		delete mapZLvls[i];

	delete[] mapZLvls;
	delete mapRenderer;
	delete rng;
}
// Needs serious work!!!!
void Map::createHeightMap(int howMountainous, float rainAmount)
{
	// Create height map of area
	// Want to normalize map eventually?
	TCODHeightMap * heightMap = new TCODHeightMap(width, height);
	heightMap->midPointDisplacement();

	//heightMap->normalize(-0.9, 0.9);

	// Add in howMountainous here so we can increase or decrease howMountainous
	double mountinStep = double(depth - MIN_LVLS_OF_ROCK);
	double heightThreshold = 100;


	// If height is below a threshold, mark that area walkable/visible. else not. 
	for (int h = 0; h < depth; ++h) {

		mapRenderer->currentZLevel = h; // Change these functions around when placing player

		for (int i = 0; i < width; ++i)
			for (int j = 0; j < height; ++j)
			{
				// Values between -100 and 100
				//int heightMapPoint = int(heightMap->getValue(i, j) * 100);
				double heightMapPoint = heightMap->getValue(i, j) * 100;

				if (h <= MIN_LVLS_OF_ROCK) {															      
					createWall({ i, j, h });
				}
				// If there's a floor but height ratio is too low, create walkable space
				else  if (heightThreshold < heightMapPoint && tileManager.getProperty<TileManager::FLOOR>({ i, j, h }))
				{   
					createWalkableSpace({ i, j, h });
				}
				// If there is a wall below, and height is high enough create more land
				else if (tileManager.canWalk({ i, j, h }))
				{                                               
					createWall({ i, j, h });
				}
				// else create open space
				else 
				{														                  
					createOpenSpace({ i, j, h });
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
			if (tileManager.getProperty<TileManager::WALL>({ i, j, depth - 1 }))
			{
				createWalkableSpace({ i, j, depth - 1 });
			}
		}

}

void Map::seedRamps()
{
	for (int h = MIN_LVLS_OF_ROCK; h < depth - 1; ++h) 
		for (int i = 0; i < width; ++i)
			for (int j = 0; j < height; ++j)
			{
				if (tileManager.getProperty<TileManager::WALL>({ i, j, h }))
				{
					const Coordinates walkableTop = { i, j, h + 1 };

					const Coordinates northWalk = { i, j + 1, h };
					const Coordinates southWalk = { i, j - 1, h };
					const Coordinates eastWalk  = { i + 1, j, h };
					const Coordinates westWalk  = { i - 1, j, h };

					if (rampSanityCheck(walkableTop, northWalk))
					{
						addRamp(northWalk);
					}
					else if (rampSanityCheck(walkableTop, southWalk))
					{
						addRamp(southWalk);
					}
					else if (rampSanityCheck(walkableTop, eastWalk))
					{
						addRamp(eastWalk);
					}
					else if (rampSanityCheck(walkableTop, westWalk))
					{
						addRamp(westWalk);
					}
				}
			}
	
}

bool Map::rampSanityCheck(Coordinates rampLoc, Coordinates rampLand) const
{
	if (tileManager.boundsCheck(rampLoc) && tileManager.canWalk(rampLoc) && tileManager.boundsCheck(rampLand) && tileManager.canWalk(rampLand))
		return true;

	return false;
}

void Map::addRamp(Coordinates co)
{
	tileManager.tileAt(co).ch = 30;
	//tileManager.tileAt(co).color = "grey";
	tileManager.setProperty<TileManager::RAMP>(co);
	tileManager.setProperty<TileManager::FLOOR>({co.x, co.y, co.z + 1});
	//tileManager.setProperty<TileManager::OBSTRUCTED>(co);
}

// Possibly move all these loops into one or two to minimize looping??
void Map::populateGrass()
{
	int grassCharList[] = { 130, 147, 244, 244 };
	int grassChar = 244;

	for (int h = MIN_LVLS_OF_ROCK; h < depth; ++h)
		for (int i = 0; i < width; ++i)
			for (int j = 0; j < height; ++j)
			{
				if (    tileManager.getProperty<TileManager::FLOOR>({ i, j, h })
					&& !tileManager.getProperty<TileManager::OBSTRUCTED>({ i, j, h }))
				{
					// Move this to EntityFactory???
					tileManager.tileAt({ i, j, h }).ch = grassCharList[rng->getInt(0, 3, 3)];
					tileManager.tileAt({ i, j, h }).color = "dark green";
				}
			}
}

// Create and use threshold for ore depending on depth
// modify threashold as depth increases to increase/decrease rarity of certain ores
void Map::populateRock()
{
	TCODNoise noise(3, TCOD_NOISE_SIMPLEX);

	// Create filler rock
	for (int h = 0; h < depth; ++h)
		for (int i = 0; i < width; ++i)
			for (int j = 0; j < height; ++j)
			{
				if (tileManager.getProperty<TileManager::WALL>({ i, j, h }))
				{
					tileManager.tileAt({ i, j, h }).ch = 133; 
					tileManager.tileAt({ i, j, h }).color = "grey";
				}
			}

	// Add ore veins to map
	VeinCreator veins(width, height, depth);
	veins.calcualteOreDepths();
	veins.addOre(tileManager);
}

void Map::addTrees(int treeDensity)
{
	// for Bearslib - 5, 6, 7 = bush, 23, 24
	// Use these eventually
	int treeChar = 24;

	int treeNum = ((width * height) / 1000) * treeDensity;

	// Store these these trees in map eventually??!!!!!!!!!!!!!!!!!!
	std::vector<Entity> trees = engine.world.createEntities(treeNum);

	TCODRandom * rng = TCODRandom::getInstance();

	int counter = 0;
	for (Entity &t : trees)
	{
		while (true) {      // Probably a better way to do this than randomly picking points in space!
			Coordinates co;
			co.x = rng->getInt(0, width-1);
			co.y = rng->getInt(0, height-1);
			co.z = rng->getInt(MIN_LVLS_OF_ROCK, depth-1);

			const Tile t = tileManager.tileAt(co);

			// If the space is clear/hasfloor plant a tree and obstruct the space
			if (t.properties & TileManager::FLOOR && !(t.properties & TileManager::OBSTRUCTED)) // Move this to EntityFactory???
			{
				trees[counter].addComponent<PositionComponent>(co);
				trees[counter].addComponent<RenderComponent>(treeChar, 0xE200, "brown");
				trees[counter].addComponent<HealthComponent>(300, 300, 0);
				trees[counter].activate();

				tileManager.setProperty<TileManager::OBSTRUCTED>(co);
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
				if (tileManager.canWalk({ i, j, h }))
				{
					engine.factory.createDwarf({ i, j, h });
					mapRenderer->currentZLevel = h;
					++dwarfNumber;
				}

				if (dwarfNumber >= number)
				{
					//engine.world.refresh();
					return;
				}
					
			}
}


// Create impassable wall that provides a floor
inline void Map::createWall(Coordinates co)
{
	mapZLvls[co.z]->setProperties(co.x, co.y, false, false);

	tileManager.setProperty<TileManager::OBSTRUCTED>(co);
	tileManager.setProperty<TileManager::WALL>(co);

	// Add a floor property
	// to any Tile above a wall Tile
	if(co.z < MAX_ZLVL - 1)
		tileManager.setProperty<TileManager::FLOOR>({co.x, co.y, co.z + 1});
}

// Create a space that is transparant and can be walked through, does not provide floor
inline void Map::createWalkableSpace(Coordinates co)
{
	mapZLvls[co.z]->setProperties(co.x, co.y, true, true);

	tileManager.setProperty<TileManager::FLOOR>(co);
	tileManager.removeProperty<TileManager::OBSTRUCTED>(co);
	tileManager.removeProperty<TileManager::WALL>(co);
}

// Create a space that is transparant and is not walkable
inline void Map::createOpenSpace(Coordinates co)
{
	mapZLvls[co.z]->setProperties(co.x, co.y, true, false);
	tileManager.removeProperty<TileManager::OBSTRUCTED>(co);
	tileManager.removeProperty<TileManager::WALL>(co);
	tileManager.removeProperty<TileManager::FLOOR>(co);
}


/*
void Map::dig(int x1, int y1, int x2, int y2)
{
	if (x2 < x1) {
		int tmp = x2;
		x2 = x1;
		x1 = tmp;
	}
	if (y2 < y1) {
		int tmp = y2;
		y2 = y1;
		y1 = tmp;
	}
	for (int tilex = x1; tilex <= x2; ++tilex)
		for (int tiley = y1; tiley <= y2; ++tiley)
			map->setProperties(tilex, tiley, true, true); // digging out walkable, transparent tiles
}

void Map::createRoom(bool first, int x1, int y1, int x2, int y2)
{
	dig(x1, y1, x2, y2);
	if (first) {
		engine.player->x = (x1 + x2) / 2; // center of first room
		engine.player->y = (y1 + y2) / 2;
	}
	else {
		TCODRandom *rng = TCODRandom::getInstance();
		int nbMonsters = rng->getInt(0, MAX_ROOM_MONSTERS); // Random num monsters between 0 and max

		while (nbMonsters > 0)
		{
			int x = rng->getInt(x1, x2); // x and y between room size
			int y = rng->getInt(y1, y2);
			if (canWalk(x, y))
				addMonster(x, y);
			--nbMonsters;
		}
	}
}

// Traverses the bsp tree creating rooms/corridors for map
class BspListener : public ITCODBspCallback {
public:
BspListener(Map & map) : map(map), roomNum(0) {}

bool visitNode(TCODBsp * node, void *userData) {
if (node->isLeaf()) { // Only creeate rooms in bsp leaf
int x, y, w, h;
// Dig the room
TCODRandom * rng = TCODRandom::getInstance();
w = rng->getInt(ROOM_MIN_SIZE, node->w - 2);
h = rng->getInt(ROOM_MIN_SIZE, node->h - 2);
x = rng->getInt(node->x + 1, node->x + node->w - w - 1);
y = rng->getInt(node->y + 1, node->y + node->h - h - 1);
map.createRoom(roomNum == 0, x, y, x + w - 1, y + h - 1);

// Dig corridor from first room
if (roomNum != 0) {
map.dig(lastx, lasty, x+w / 2, lasty);
map.dig(x+w / 2, lasty, x+w / 2, y+h / 2);
}
// Dig corridor from center to last room
lastx = x + w / 2;
lasty = y + h / 2;
roomNum++;
}
return true;
}
private:
Map & map;
int roomNum;
int lastx, lasty; // Center of last room, used for corridor digging
};



void Map::addMonster(int x, int y)
{
TCODRandom *rng = TCODRandom::getInstance();
if (rng->getInt(0, 100) < 80)
{
Actor *orc = new Actor(x, y, 'o', "orc", TCODColor::desaturatedGreen);
orc->destructible = new MonsterDestructible(10, 0, "Dead Orc");
orc->attacker = new Attacker(3);
orc->ai = new MonsterAi();

engine.actors.push(orc);
}
else
{
Actor * troll = new Actor(x, y, 'T', "troll", TCODColor::darkerGreen);
troll->destructible = new MonsterDestructible(16, 1, "Troll carcas");
troll->attacker = new Attacker(4);
troll->ai = new MonsterAi();

engine.actors.push(troll);
}
}
*/