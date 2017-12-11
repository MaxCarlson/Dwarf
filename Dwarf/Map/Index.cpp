#include "Index.h"
#include "Tile.h"
#include <iostream>

#define BOUNDS_CHECKING

int getIdx(Coordinates co)
{
#ifdef BOUNDS_CHECKING

	if (co.z >= MAP_DEPTH || co.z < 0 || co.y >= MAP_HEIGHT || co.y < 0 || co.x >= MAP_WIDTH || co.x < 0)
		std::cout << "Map Idx Bounds Fail!!!" << std::endl;

#endif // BOUNDS_CHECKING

	return TILE_ARRAY_LOOKUP;
}

Coordinates idxToCo(int idx)
{
	int z = idx / (MAP_HEIGHT * MAP_WIDTH);
	idx -= (z * MAP_WIDTH * MAP_HEIGHT);

	int y = idx / MAP_WIDTH;
	idx -= (y * MAP_WIDTH);

	int x = idx;

	return { x, y, z };
}
