#include "stdafx.h"
#include "plant_trees.h"
#include "../../Tile.h"
#include "../../../Helpers/Rng.h"
#include "Coordinates.h"

inline void setTrunk(Coordinates co)
{

}

inline void plantDeciduous(Rng &rng, Coordinates co)
{

}

inline void plantEvergreen(Rng &rng, Coordinates co)
{

}

bool isCanopy(Coordinates co)
{
	for (int z = co.z; z < MAP_DEPTH - 1; ++z)
	{
		if (region::solid(getIdx({ co.x, co.y, z })))
			return true;
	}

	return false;
}

void plantTrees(Rng &rng)
{
	const int decChance = 500;
	const int everChance = 500;

	for (int x = 10; x < MAP_WIDTH - 10; ++x)
		for(int y = 10; y < MAP_HEIGHT - 10; ++y)
		{
			const int z = region::groundLevel(x, y);
			const int idx = getIdx({ x, y, z });

			if (region::getTileType(idx) == region::TileTypes::FLOOR && !isCanopy({x, y, z}))
			{
				int roll = rng.range(1, 1000);

				if (roll <= everChance)
				{
					plantEvergreen(rng, { x, y, z });
				}
				else
				{
					roll = rng.range(1, 1000);

					if (roll <= decChance)
					{
						plantDeciduous(rng, { x, y, z });
					}
				}
			}
		}
}
