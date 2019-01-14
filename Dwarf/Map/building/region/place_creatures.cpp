#include "place_creatures.h"
#include "../../Tile.h"
#include "../ECS/Components/PositionComponent.h"
#include "Globals\Camera.h"
#include "ECS\Messages\spawn_creature_message.h"
#include "Helpers\Rng.h"
#include "Globals\GlobalWorld.h"
#include "Raws\FileReader.h"
#include <SFML\Graphics\Color.hpp>

using region::TileTypes;
using region::getTileType;

constexpr int extraTiles = 5;

inline const std::string& randomName()
{
	static const auto& namesVec = FileStrings::returnTable(0)->strings;
	return namesVec[rng.range(0, namesVec.size() - 1)];
}

template<class Func>
bool spiralOut(Coordinates co, int tiles, int maxBlockingTiles, Func&& lam)
{
	bool toX			= true;
	bool add			= true;
	int addVal			= 1;
	int count			= 0;
	int blockingTiles	= 0;
	std::function func	= std::plus<int>();

	int i = 0;
	for (; i < tiles && blockingTiles < maxBlockingTiles;)
	{
		if (!region::boundsCheck(co))
			break;

		if (getTileType(getIdx(co)) == TileTypes::FLOOR)
		{
			++i;
			lam(co);
		}
		else
			++blockingTiles;

		if (toX)
			co.x = func(co.x, addVal);
		else
			co.y = func(co.y, addVal);
		toX = !toX;

		if (++count % 2 == 0)
		{
			if (add)
				func = std::minus<int>();
			else
				func = std::plus<int>();
			add = !add;
			++addVal;
		}
	}

	return i >= tiles;
}

bool scanForSuitable(int tiles, int z, std::vector<Coordinates>& suitableTiles)
{
	int trys = (MAP_WIDTH * MAP_HEIGHT) / tiles + 10;
	int maxBlockingTiles = tiles * 3;

	for (int t = 0; t < trys; ++t)
	{
		int x			= rng.range(3, MAP_WIDTH - 4);
		int y			= rng.range(3, MAP_WIDTH - 4);
		Coordinates co	= { x, y, z };

		// Choose a random spot and spiral out,
		// checking to see if there are enough tiles in the general area for the dwarves
		if (spiralOut(co, tiles, maxBlockingTiles, [](auto co) {}))
		{
			spiralOut(co, tiles, maxBlockingTiles, [&](Coordinates co)
			{
				suitableTiles.emplace_back(co);
			});
			break;
		}
		
	}
	return !suitableTiles.empty();
}

void placeDwarves(int numDwarves)
{
	bool found		= false;
	int freeTiles	= numDwarves + extraTiles;
	std::vector<Coordinates> suitableTiles;

	// TODO: grab these from Species
	static const vchar dwarfVchar{ 769, color_from_name("default"), sf::Color::Transparent.toInteger() };
	static const float moveSpeed = 3.5;

	// Scan the map for an area with enough space to place our dwarves + their belongings
	for (int h = MAP_DEPTH - 1; h > 1; --h)
		for (int i = 3; i < MAP_WIDTH - 2; ++i)
			for (int j = 3; j < MAP_HEIGHT - 2; ++j)
				if (getTileType(getIdx({ i, j, h })) == TileTypes::FLOOR
					&& scanForSuitable(freeTiles, h, suitableTiles))
				{
					found = true;
					goto endLoop;
				}

	endLoop:

	if (!found) // TODO: Handle this error by bringing us back to map gen
		throw std::runtime_error("Failed to place dwarves on map");


	// TODO: Use the remaining elements in this vector to place the dwaves stuff
	std::shuffle(std::begin(suitableTiles), std::end(suitableTiles), std::default_random_engine{});
	for (int i = 0; i < numDwarves; ++i)
	{
		auto co = suitableTiles.back();
		suitableTiles.pop_back();
		world.emit_deferred(spawn_creature_message{ "dwarf", randomName(), randomName(), dwarfVchar, co, moveSpeed, true });

		// TODO: Maybe set camera based on most common level instead of last placed?
		camera.z = co.z;
	}

	/*
	int dwarfNumber = 0;
	auto number = dwarfContainer.dwarves.size(); //engine->Dwarves.size();

	for (int h = MAP_DEPTH - 1; h > 1; --h)
		for (int i = 3; i < MAP_WIDTH - 2; ++i)
			for (int j = 3; j < MAP_HEIGHT - 2; ++j)
			{
				if (getTileType(getIdx({ i, j, h })) == TileTypes::FLOOR)
				{
					dwarfContainer.dwarves.at(dwarfNumber).getComponent<PositionComponent>().co = { i, j, h };
					camera.z = h;
					++dwarfNumber;
				}

				if (dwarfNumber >= number)
				{
					return;
				}
			}
	*/
}
