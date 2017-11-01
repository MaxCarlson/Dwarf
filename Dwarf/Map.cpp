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

	// Create map general shape
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

					else  if (heightMap->getValue(i, j) < heightRatio && isFloor({ i, j, h })) {  // If floor below but height ratio is too low, create walkable space
						createWalkableSpace({ i, j, h });
					}
					else if (canWalk({ i, j, h })) {                                               // If there is a wall below, and height is high enough create more mountain
						createWall({ i, j, h });
					}
					else {														              // else create open space
						createOpenSpace({ i, j, h });
					}

				}

			heightRatio += 0.2;
		}

		// If player can't walk, search within the middle third of the map, from the top down for a place where they can
		bool found = false;
		for (int h = depth - 1; h > 0; --h) {
			if (found) break;
			map = mapZLvls[h];
			for (int i = width / 3; i < width * 0.66; ++i) {
				if (found) break;
				for (int j = height / 3; j < height * 0.66; ++j) 
				{
					if (canWalk({ i, j, h })) {
						engine.player->co = { i, j, h };
						map = mapZLvls[h];
						found = true;
					}
					if (found) break;
				}
			}
		}
			
			
		//}

		delete heightMap;
		
	} while (!mapIsOkay() 
		|| !canWalk(engine.player->co));

	populateRock();

}

Map::~Map()
{
	//delete[] tiles;
	delete map;
	for (int i = 0; i < MAX_ZLVL; ++i)
		delete mapZLvls[i];

	delete mapZLvls;
}

void Map::populateRock()
{
	for (int h = 0; h < depth; ++h) 
		for (int i = 0; i < width; ++i)
			for (int j = 0; j < height; ++j)
			{
				tileAt({ i, j, h })->actors.resize(1);
				if (isWall({ i, j, h })) {
					tileAt({ i, j, h })->actors.push_back(new Stone({ i, j, h }, '%', STONE_ID, "granite", TCODColor::lightSepia, Stone::GRANITE));
				}
			}
}

// Ensure map has between MAX_MAP_FILL and MIN_MAP_FILL % squares full 
bool Map::mapIsOkay() const
{
	int walkAbleCount = 0;
	const int max = width * height;

	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j) {
			if (canWalk({ i, j, currentZLevel }))
				++walkAbleCount;
		}
	
	float fillRatio = float(max - walkAbleCount) / float(max);
	if ( fillRatio > MAX_MAP_FILL || fillRatio < MIN_MAP_FILL )
		return false;
	

	return true;
}

bool Map::isWall(Coordinates co) const
{
	return tileAt(co)->isWall;
}

bool Map::canWalk(Coordinates co) const
{
	if (!isFloor(co) || isWall(co))
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
bool Map::isFloor(Coordinates co) const
{
	if (tileBelow(co)->providesFloor)
		return true;

	return false;
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
	map->setProperties(co.x, co.y, false, false);
	tileAt(co)->providesFloor = true;
	tileAt(co)->obstructed    = true;
	tileAt(co)->isWall        = true;
}

// Create a space that is transparant and can be walked through, does not provide floor
inline void Map::createWalkableSpace(Coordinates co)
{
	map->setProperties(co.x, co.y, true, true);  
	tileAt(co)->providesFloor = false;
	tileAt(co)->obstructed    = false;
	tileAt(co)->isWall        = false;
}

// Create a space that is transparant and is not walkable
inline void Map::createOpenSpace(Coordinates co)
{
	map->setProperties(co.x, co.y, true, false);  
	tileAt(co)->providesFloor = false;
	tileAt(co)->obstructed    = false;
	tileAt(co)->isWall        = false;
}

void Map::computeFov()
{
	map->computeFov(engine.player->co.x, engine.player->co.y, engine.fovRadius);
}

void Map::render() const
{
	static const TCODColor darkWall(0, 0, 100);
	static const TCODColor darkGround(50, 50, 150);
	static const TCODColor lightWall(130, 110, 50);
	static const TCODColor lightGround(200, 180, 50);

	// TCODRandom * rng = new TCODRandom(); if (rng->getInt(0, 8) == 8) TCODConsole::root->setChar(x, y, ','); (hard code these values once genrated)

	// Iterates through map squares
	for(int x = 0; x < width; ++x)
		for (int y = 0; y < height; ++y)
		{
			//const Tile t = *tileAt({ x, y, currentZLevel });
			//if (t.actors[0] && t.actors[0]->a_id == STONE_ID)
			//{
			//	TCODConsole::root->setCharBackground(x, y, t.actors[0]->col);
			//	TCODConsole::root->setChar(x, y, t.actors[0]->ch);
			//}
			if (isInFov(x, y))
				TCODConsole::root->setCharBackground(x, y, isWall({ x, y, currentZLevel }) ? lightWall : lightGround);
			else if (!isFloor({ x, y, currentZLevel })) {
				TCODConsole::root->setCharBackground(x, y, TCODColor::darkestGrey); 
			}
			else if(isExplored(x, y))
				TCODConsole::root->setCharBackground(x, y, isWall({ x, y, currentZLevel }) ? darkWall  : darkGround);


			//delete t;
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