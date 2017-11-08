#include "Map.h"

#include "Engine.h"
#include "Tile.h"
#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\Components\HealthComponent.h"


static const float MAX_MAP_FILL = 0.85;
static const float MIN_MAP_FILL = 0.18;
static const int MIN_LVLS_OF_ROCK = 3;

Map::Map(int width, int height, int depth) : width(width), height(height), depth(depth), tileManager(width, height, depth)
{

	for (int i = 0; i < MAX_ZLVL; ++i) {
		mapZLvls[i] = new TCODMap(width, height);
	}

	createHeightMap(16, 0.3f);
	seedRamps();
	populateRock();
	addTrees(10);
}

Map::~Map()
{
	delete map;
	for (int i = 0; i < MAX_ZLVL; ++i)
		delete mapZLvls[i];

	delete mapZLvls;
}

void Map::createHeightMap(int howMountainous, float rainAmount)
{
	// Create map general shape

	TCODRandom * rng = TCODRandom::getInstance();
	// Create height map of area
	TCODHeightMap * heightMap = new TCODHeightMap(129, 129);
	heightMap->midPointDisplacement();
	heightMap->rainErosion(width*height * rainAmount, 0.1, 0.1, rng);

	//float heightRatio = -0.6f;
	// If height is below a threshold, mark that area walkable/visible. else not. 
	for (int h = 0; h < depth; ++h) {

		map = mapZLvls[h]; // Set map to map representing current depth
		currentZLevel = h; // Change these functions around when placing player

		for (int i = 0; i < width; ++i)
			for (int j = 0; j < height; ++j)
			{
				// Values between -100 and 100
				int heightMapPoint = int(heightMap->getValue(i, j) * 100);

				if (h <= MIN_LVLS_OF_ROCK) {															       // Below level four set map to always be filled
					createWall({ i, j, h });
				}

				//else  if (heightMap->getValue(i, j) < heightRatio && tileManager.getProperty<TileManager::WALL>({ i, j, h - 1 })) {   // If floor below but height ratio is too low, create walkable space
				//	createWalkableSpace({ i, j, h });
				//}
				else  if (heightMapPoint < howMountainous && tileManager.getProperty<TileManager::WALL>({ i, j, h - 1 })) {   // If floor below but height ratio is too low, create walkable space
					createWalkableSpace({ i, j, h });
				}
				else if (tileManager.canWalk({ i, j, h })) {                                               // If there is a wall below, and height is high enough create more mountain
					createWall({ i, j, h });
				}
				else {														                   // else create open space
					createOpenSpace({ i, j, h });
				}

			}
		howMountainous += howMountainous / 10;
		//heightRatio += 0.2;
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
	//for (int h = 0; h < depth; ++h) {
	//	for (int i = 0; i < width; ++i)
	//		for (int j = 0; j < height; ++j) 
	//		{
	//			if
	//		}
}

void Map::populateRock()
{
	/*
	for (int h = 0; h < depth; ++h) 
		for (int i = 0; i < width; ++i)
			for (int j = 0; j < height; ++j)
			{
				tileAt({ i, j, h })->actors.resize(1);
				if (isWall({ i, j, h })) {
					tileAt({ i, j, h })->actors.push_back(new Stone({ i, j, h }, '%', STONE_ID, "granite", TCODColor::lightSepia, Stone::GRANITE));
				}
			}
	*/
}

void Map::addTrees(int treeDensity)
{
	int treeNum = ((width * height) / 1000) * treeDensity;

	// 70 = char num for trees

	std::vector<Entity> trees = engine.world.createEntities(treeNum);

	// Don't try to place trees below MIN rock level
	int startLevel = MIN_LVLS_OF_ROCK * width * height + 0 * width + 0;
	int maxLevel = depth * width * height - 1;

	TCODRandom * rng = TCODRandom::getInstance();

	
	int counter = 0;
	for (Entity &t : trees)
	{
		while (true) {
			Coordinates co;
			co.x = rng->getInt(0, width-1);
			co.y = rng->getInt(0, height-1);
			co.z = rng->getInt(MIN_LVLS_OF_ROCK, depth-1);

			const Tile t = tileManager.tileAt(co);


			if (t.properties & TileManager::FLOOR && !(t.properties & TileManager::OBSTRUCTED))
			{
				trees[counter].addComponent<PositionComponent>(co);
				trees[counter].addComponent<RenderComponent>(80, TCODColor::green, TCODColor::darkGreen);
				trees[counter].addComponent<HealthComponent>(300, 300, 0);
				trees[counter].activate();

				tileManager.setProperty<TileManager::OBSTRUCTED>(co);
				break;
			}

		}
		++counter;
	}

}

// Ensure map has between MAX_MAP_FILL and MIN_MAP_FILL % squares full 
bool Map::mapIsOkay() const
{
	int walkAbleCount = 0;
	const int max = width * height;

	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j) {
			if (tileManager.canWalk({ i, j, currentZLevel }))
				++walkAbleCount;
		}
	
	float fillRatio = float(max - walkAbleCount) / float(max);
	if ( fillRatio > MAX_MAP_FILL || fillRatio < MIN_MAP_FILL )
		return false;
	
	return true;
}

// Create impassable wall that provides a floor
inline void Map::createWall(Coordinates co)
{
	map->setProperties(co.x, co.y, false, false);

	tileManager.setProperty<TileManager::OBSTRUCTED>(co);
	tileManager.setProperty<TileManager::WALL>(co);
	if(co.z < MAX_ZLVL - 1)
		tileManager.setProperty<TileManager::FLOOR>({co.x, co.y, co.z + 1});
}

// Create a space that is transparant and can be walked through, does not provide floor
inline void Map::createWalkableSpace(Coordinates co)
{
	map->setProperties(co.x, co.y, true, true);  

	tileManager.setProperty<TileManager::FLOOR>(co);
	tileManager.removeProperty<TileManager::OBSTRUCTED>(co);
	tileManager.removeProperty<TileManager::WALL>(co);
}

// Create a space that is transparant and is not walkable
inline void Map::createOpenSpace(Coordinates co)
{
	map->setProperties(co.x, co.y, true, false);  
	tileManager.removeProperty<TileManager::OBSTRUCTED>(co);
	tileManager.removeProperty<TileManager::WALL>(co);
	tileManager.removeProperty<TileManager::FLOOR>(co);
}

void Map::render() const
{
	static const TCODColor darkWall(0, 0, 100);
	static const TCODColor darkGround(50, 50, 150);
	static const TCODColor lightWall(130, 110, 50);
	static const TCODColor lightGround(200, 180, 50);

	// TCODRandom * rng = new TCODRandom(); if (rng->getInt(0, 8) == 8) TCODConsole::root->setChar(x, y, ','); (hard code these values once genrated)

	// Iterates through visible map squares
	for(int x = 0; x < width; ++x)
		for (int y = 0; y < height; ++y)
		{
			if (!tileManager.getProperty<TileManager::FLOOR>({ x, y, currentZLevel })) {
				TCODConsole::root->setCharBackground(x, y, TCODColor::darkestGrey);
			}
			else if (tileManager.getProperty<TileManager::EXPLORED>({ x, y, currentZLevel }))
				TCODConsole::root->setCharBackground(x, y, (tileManager.getProperty<TileManager::WALL>({ x, y, currentZLevel }) ? darkWall : darkGround));
		}
	//TCOD_image_t  pix = TCOD_image_load("Obsidian_16x16.png");

	//static TCODImage  pix("../Obsidian_16x16.png");
	

	// Note to self: sx = top left px, sy = y px location
	//pix.blit2x(TCODConsole::root, 10, 10, 32, 0, 16, 16);
	//pix.blit(TCODConsole::root, 0.3, 0.3, TCOD_BKGND_SET, .1, .1, 0);
	//pix.
	//TCOD_image_blit_rect(pix, TCODConsole::root, 3,5, 4, 4, TCOD_BKGND_NONE);
}

// Change the map being rendered (simulating z levels with map array)
bool Map::incrementZLevel(int inc)
{
	if (currentZLevel + inc >= 0 && currentZLevel + inc < MAX_ZLVL) {
		map = mapZLvls[currentZLevel + inc];
		currentZLevel += inc;
		return true;
	}
	return false;
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