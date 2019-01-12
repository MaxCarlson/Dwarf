#include "place_creatures.h"
#include "../../Tile.h"
#include "../ECS/Components/PositionComponent.h"
//#include "Globals\DwarfContainer.h"
#include "Globals\Camera.h"
#include "ECS\Messages\spawn_creature_message.h"
#include "Helpers\Rng.h"

using region::TileTypes;
using region::getTileType;

constexpr int extraTiles = 5;

bool scanAndPlace(int tiles, int z)
{
	int trys = (MAP_WIDTH * MAP_HEIGHT) / tiles + 10;

	for (int t = 0; t < trys; ++t)
	{
		int x = rng.range(3, MAP_WIDTH - 4);
		int y = rng.range(3, MAP_WIDTH - 4);

		Coordinates co = { x, y, z };

		bool toX	= true;
		bool add	= true;
		int addVal	= 1;
		std::function func	= std::plus<int>();
		int count	= 0;

		for (int i = 1; i < tiles; ++i)
		{
			if (region::boundsCheck(co)
				&& getTileType(getIdx(co)) == TileTypes::FLOOR)
			{
				if (toX)
					func(co.x, addVal);
				else
					func(co.y, addVal);
				toX = !toX;

				if (++count % 2 == 0)
				{
					if (add)
						func = std::minus<int>();
					else
						func = std::plus<int>();
					add		= !add;
					++addVal;
				}
			}
			else
				break;
		}

		
	}
}

// Re-work this!!!
void placeDwarves(int numDwarves)
{
	bool found = false;
	int freeTiles = numDwarves + extraTiles;

	// Scan the map for an area with enough space to place our dwarves + their belongings
	for (int h = MAP_DEPTH - 1; h > 1; --h)
		for (int i = 3; i < MAP_WIDTH - 2; ++i)
			for (int j = 3; j < MAP_HEIGHT - 2; ++j)
				if (getTileType(getIdx({ i, j, h })) == TileTypes::FLOOR
					&& scanAndPlace(freeTiles, h))
				{
					found = true;
					break;
				}

	if (!found) // TODO: Handle this error by bringing us back to map gen
		throw std::runtime_error("Failed to place dwarves on map");


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
