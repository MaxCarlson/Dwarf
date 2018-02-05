#include "place_creatures.h"
#include "../../Tile.h"
#include "../ECS/Components/PositionComponent.h"
#include "Globals\DwarfContainer.h"
#include "Globals\Camera.h"

using region::TileTypes;
using region::getTileType;

// Re-work this!!!
void placeDwarves()
{
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
}
