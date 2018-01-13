#include "stdafx.h"
#include "plant_trees.h"
#include "../../Tile.h"
#include "../../../Helpers/Rng.h"
#include "Coordinates.h"

using region::TileTypes;

inline void setTrunk(Coordinates co, const int treeId)
{
	if (co.x > 0 && co.y > 0 && co.z > 0 && co.x < MAP_WIDTH - 1 && co.y < MAP_HEIGHT - 1 && co.z < MAP_DEPTH - 2)
	{
		const int idx = getIdx(co);

		region::setTileType(idx, TileTypes::TREE_TRUNK);
		region::setTreeId(idx, treeId);
		region::setSolid(idx);
	}
}

inline void setFoliage(Coordinates co, const int treeId)
{
	if (co.x > 0 && co.y > 0 && co.z > 0 && co.x < MAP_WIDTH - 1 && co.y < MAP_HEIGHT - 1 && co.z < MAP_DEPTH - 2)
	{
		const int idx = getIdx(co);

		region::setTileType(idx, TileTypes::TREE_LEAF);
		region::setTreeId(idx, treeId);
		region::setSolid(idx);
	}
}

inline void setTreeHp(const int treeId, const int height)
{
	region::setTreeHealth(treeId, height * 250);
}

inline void plantDeciduous(Rng &rng, Coordinates co)
{
	const auto id = region::nextTreeId();
	const int height = rng.range(2, 6);

	for (int i = 0; i < height; ++i)
	{
		setTrunk({ co.x, co.y, co.z + i }, id);

		if (i > height / 2)
		{
			const int radius = (height - i) + 1;

			for (int x = co.x - radius; x < co.x + radius; ++x)
				for (int y = co.y - radius; y < co.y + radius; ++y)
				{
					const auto dist = region::get_2D_distance({ x, y, 1 }, { co.x, co.y, 1 });

					if (dist <= radius && (x != co.x || y != co.y))
						setFoliage({ x, y, co.z + i }, id);
				}
		}
	}

	setTreeHp(id, height);
}

inline void plantEvergreen(Rng &rng, Coordinates co)
{
	const auto id = region::nextTreeId();
	const int height = rng.range(2, 6);

	for (int i = 0; i < height; ++i)
	{
		setTrunk({ co.x, co.y, co.z + i }, id);

		if (i > 0)
		{
			const int radius = (height - i) / 2 + 1;
			
			for(int x = co.x - radius; x < co.x + radius; ++x)
				for (int y = co.y - radius; y < co.y + radius; ++y)
				{
					const auto dist = region::get_2D_distance({ x, y, 1 }, { co.x, co.y, 1 });

					if (dist <= radius && (x != co.x || y != co.y))
						setFoliage({ x, y, co.z + i }, id);
				}
		}
	}

	setTreeHp(id, height);
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
	const int decChance = 15;
	const int everChance = 30;

	for (int x = 10; x < MAP_WIDTH - 10; ++x)
		for(int y = 10; y < MAP_HEIGHT - 10; ++y)
		{
			const int z = region::groundLevel(x, y);
			const int idx = getIdx({ x, y, z });

			if (region::getTileType(idx) == TileTypes::FLOOR && !isCanopy({x, y, z}))
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
