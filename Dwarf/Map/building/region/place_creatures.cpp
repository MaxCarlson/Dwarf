#include "place_creatures.h"
#include "../../Tile.h"
#include "../Engine.h"
#include "../ECS/Components/PositionComponent.h"

using region::TileTypes;
using region::getTileType;

// Re-work this!!!
void placeDwarves()
{
	int dwarfNumber = 0;
	std::size_t number = engine->Dwarves.size();

	for (int h = MAP_DEPTH - 1; h > 1; --h)
		for (int i = 3; i < MAP_WIDTH - 2; ++i)
			for (int j = 3; j < MAP_HEIGHT - 2; ++j)
			{
				if (getTileType(getIdx({ i, j, h })) == TileTypes::FLOOR)
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
