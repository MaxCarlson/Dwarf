#include "buildings.h"
#include "../../Tile.h"

using region::boundsCheck;
using region::TileTypes;
using region::getTileType;
using region::makeRamp;

bool rampSanityCheck(Coordinates rampLoc, Coordinates rampLand)
{
	if (boundsCheck(rampLoc) && getTileType(getIdx(rampLoc)) == TileTypes::FLOOR && boundsCheck(rampLand) && getTileType(getIdx(rampLand)) == TileTypes::FLOOR)
		return true;

	return false;
}

void placeRamps()
{
	for (int h = 10; h < MAP_DEPTH - 1; ++h)
		for (int i = 0; i < MAP_WIDTH; ++i)
			for (int j = 0; j < MAP_HEIGHT; ++j)
			{
				if (getTileType(getIdx({ i, j, h })) == TileTypes::SOLID)
				{
					const Coordinates walkableTop = { i, j, h + 1 };

					const Coordinates northWalk = { i, j + 1, h };
					const Coordinates southWalk = { i, j - 1, h };
					const Coordinates eastWalk = { i + 1, j, h };
					const Coordinates westWalk = { i - 1, j, h };

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