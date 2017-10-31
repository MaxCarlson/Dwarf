#include "Map.h"
#include "Engine.h"
#include "Actor.h"
#include "Destructible.h"
#include "Attacker.h"
#include "Ai.h"
#include "Tile.h"


static const float MAX_MAP_FILL = 0.85;
static const float MIN_MAP_FILL = 0.18;

Map::Map(int width, int height, int depth) : width(width), height(height), depth(depth)
{

	for (int i = 0; i < MAX_ZLVL; ++i) {
		mapZLvls[i] = new TCODMap(width, height);
		tiles[i] = new Tile[width * height];
	}


	do {
		TCODRandom * rng = TCODRandom::getInstance();
		// Create height map of area
		TCODHeightMap * heightMap = new TCODHeightMap(129, 129);
		heightMap->midPointDisplacement();

		float heightRatio = -0.6;
		// If height is below a threshold, mark that area walkable/visible. else not. 
		for (int h = 0; h < depth; ++h) {

			map = mapZLvls[h]; // Set map to map representing current depth
			currentZLevel = h; // Change these functions around when placing player
			

			for (int i = 0; i < width; ++i)
				for (int j = 0; j < height; ++j)

				{
					if (h <= 3) {																// Below level four set map to always be filled
						createWall({ i, j, h });
					}

					else  if (heightMap->getValue(i, j) < heightRatio && isFloor(i, j, h)) {  // If height is high enough, create more mountain
						createWall({ i, j, h });
					}
					else if (canWalk(i, j, h)){               // If floor below but height ratio is too low, create walkable space
						createWalkableSpace({ i, j, h });
					}
					else {														              // else create open space
						createOpenSpace({ i, j, h });
					}

				}

			heightRatio += 0.2;
		}

		// Make sure the player can walk on map load!
		while (!canWalk(engine.player->x, engine.player->y, engine.player->z))
		{
			engine.player->x = rng->getInt(0, width);
			engine.player->y = rng->getInt(0, height);
			engine.player->z = rng->getInt(0, depth);
			map = mapZLvls[engine.player->z];
		}


		delete heightMap;
		
	} while (!mapIsOkay());



	//TCODBsp bsp(0, 0, width, height);
	//bsp.splitRecursive(NULL, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
	//BspListener listener(*this);
	//bsp.traverseInvertedLevelOrder(&listener, NULL);
}

Map::~Map()
{
	//delete[] tiles;
	delete map;
	for (int i = 0; i < MAX_ZLVL; ++i)
		delete mapZLvls[i];

	delete mapZLvls;
}

// Ensure map has between MAX_MAP_FILL and MIN_MAP_FILL % squares full 
bool Map::mapIsOkay() const
{
	int walkAbleCount = 0;
	const int max = width * height;

	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j) {
			if (map->isWalkable(i, j))
				++walkAbleCount;
		}
	
	float fillRatio = float(max - walkAbleCount) / float(max);
	if ( fillRatio > MAX_MAP_FILL || fillRatio < MIN_MAP_FILL )
		return false;
	

	return true;
}

bool Map::isWall(int x, int y) const
{
	return !map->isWalkable(x, y);
}

bool Map::canWalk(int x, int y, int z) const
{
	if (!isFloor(x, y, z))
		return false;
	/*
	for (Actor * actor : engine.actors)
	{
		if ( actor->blocks && actor->x == x && actor->y == y) // Cannot walk through blocking actors. Optimization, add a value to tile occupied. test this instead with direct lookup
			return false;
	}
	*/
	return true;
}

// Test if the tile underneath input coordinates provides a floor
bool Map::isFloor(int x, int y, int z) const
{
	if (!(tiles[z - 1] + (x + y * height))->providesFloor)
		return false;

	return true;
}

bool Map::isInFov(int x, int y) const
{
	// Handle out of fov values
	if (x < 0 || x >= width || y < 0 || y >= height)
		return false;

	if (map->isInFov(x, y)) {
		(tiles[currentZLevel] + (x + y * width))->explored = true;
		return true;
	}
	return false;
}

bool Map::isExplored(int x, int y) const
{
	return (tiles[currentZLevel] + (x + y * width))->explored;
}

// Create impassable wall that provides a floor
inline void Map::createWall(Coordinates co)
{
	map->setProperties(co.x, co.y, false, false); // These maps need be removed or revamped!
	tileAt(co.x, co.y, co.z)->providesFloor = true;
	tileAt(co.x, co.y, co.z)->obstructed = true;
}

// Create a space that is transparant and can be walked through, does not provide floor
inline void Map::createWalkableSpace(Coordinates co)
{
	map->setProperties(co.x, co.y, true, true);  // These maps need be removed or revamped!
	tileAt(co.x, co.y, co.z)->providesFloor = false;
	tileAt(co.x, co.y, co.z)->obstructed = false;
}

// Create a space that is transparant and is not walkable
inline void Map::createOpenSpace(Coordinates co)
{
	map->setProperties(co.x, co.y, true, false);  // These maps need be removed or revamped!
	tileAt(co.x, co.y, co.z)->providesFloor = false;
	tileAt(co.x, co.y, co.z)->obstructed = false;
}

void Map::computeFov()
{
	map->computeFov(engine.player->x, engine.player->y, engine.fovRadius);
}

void Map::render() const
{
	static const TCODColor darkWall(0, 0, 100);
	static const TCODColor darkGround(50, 50, 150);
	static const TCODColor lightWall(130, 110, 50);
	static const TCODColor lightGround(200, 180, 50);

	// Iterates through map squares
	for(int x = 0; x < width; ++x)
		for (int y = 0; y < height; ++y)
		{
			if (isInFov(x, y))
				TCODConsole::root->setCharBackground(x, y, isWall(x, y) ? lightWall : lightGround);
			else if(isExplored(x, y))
				TCODConsole::root->setCharBackground(x, y, isWall(x, y) ? darkWall  : darkGround);
		}
}

// Change the map being rendered (simulating z levels with map array)
void Map::incrementZLevel(int inc)
{
	if (currentZLevel + inc >= 0 && currentZLevel + inc < MAX_ZLVL) {
		map = mapZLvls[currentZLevel + inc];
		currentZLevel += inc;
	}
}
// Jumps to z level entered if within bounds of map array
void Map::jumpToZLevel(int level)
{
	if (level >= 0 && level < MAX_ZLVL) {
		map = mapZLvls[level];
		currentZLevel = level;
	}

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