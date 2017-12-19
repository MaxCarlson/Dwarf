#include "rocklayer.h"
#include "../../Tile.h"
#include "../Raws/Materials.h"
#include "../Raws/Defs/MaterialDef.h"
#include <libtcod.hpp>
#include <algorithm>

using region::TileTypes;

void buildStrata(FastNoise &noise, TCODRandom & rng)
{
	Strata strata;

	strata.strata_map.resize(TOTAL_MAP_TILES);
}

void layRock(std::vector<uint8_t> heightMapt, TCODRandom & rng)
{

	for (int i = 0; i < MAP_WIDTH; ++i)
		for (int j = 0; j < MAP_HEIGHT; ++j)
		{
			int z = 1;
			const int cell_idx = (j * MAP_WIDTH) + i;
			int alt = heightMapt[cell_idx];

			// Add the Rock/Ore's layer
			while (z < std::min(alt + 12, MAP_DEPTH - 15))
			{
				const int idx = getIdx({ i, j, z });
				region::setTileType(idx, TileTypes::SOLID);

				region::setMaterial({ i, j, z }, getMaterialIdx("granite_rock")); // Replace this with strata building once up and running

				++z;
			}

			const int idx = getIdx({ i, j, z });
			region::setTileType(idx, TileTypes::FLOOR);

			// Set vegitation ~~ sepperate vegitation from materials

			std::string tmp = "grass";
			int rg = rng.getInt(1, 3, 3);

			tmp += rg + '0';

			//tileFactory->createGrass({ i, j, h });
			region::setMaterial({ i, j, z }, getMaterialIdx(tmp));

			++z;

			// Set soil and floor layers
			//while (z < MAP_DEPTH - 2)
			//{
			//	++z;
			//}

			// Set empty space with remaing space
			while (z < MAP_DEPTH - 1)
			{
				const int idx = getIdx({ i, j, z });
				region::setTileType(idx, TileTypes::EMPTY_SPACE);
				++z;
			}
		}
}