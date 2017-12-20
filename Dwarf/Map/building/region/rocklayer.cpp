#include "rocklayer.h"
#include "../../Tile.h"
#include "../Raws/Materials.h"
#include "../Raws/Defs/MaterialDef.h"
#include "../Helpers/FastNoise.h"
#include <libtcod.hpp>
#include <algorithm>


using region::TileTypes;

Strata buildStrata(std::vector<uint8_t>& heightMap, FastNoise & noise, TCODRandom & rng)
{
	Strata strata;
	strata.strata_map.resize(TOTAL_MAP_TILES);

	std::vector<std::size_t> soils;
	std::vector<std::size_t> sands;
	std::vector<std::size_t> sedimintaries;
	std::vector<std::size_t> igneous;
	std::vector<std::size_t> metamorphics;

	getStrataLayers(soils, sands, igneous, sedimintaries, metamorphics);

	const int numStrata = (MAP_WIDTH + MAP_HEIGHT) * 4 + rng.getInt(1, 64);

	strata.strata_map.resize(TOTAL_MAP_TILES);
	strata.material_idx.resize(numStrata);

	std::fill(strata.strata_map.begin(), strata.strata_map.end(), 1);
	std::fill(strata.material_idx.begin(), strata.material_idx.end(), 1);
	std::fill(strata.counts.begin(), strata.counts.end(), std::make_tuple<int, int, int, int>(0, 0, 0, 0));
	strata.counts.resize(numStrata);

	FastNoise strataNoise(rng.getInt(1, 19999)); // Replace with planet seed once implemented

	strataNoise.SetNoiseType(FastNoise::Cellular);

	//strataNoise.SetCellularDistanceFunction(FastNoise::Natural);
	//strataNoise.SetCellularReturnType(FastNoise::CellularReturnType::Distance2Sub);
	//strataNoise.SetCellularJitter(0.995f);

	for (int z = 0; z < MAP_DEPTH; ++z) {
		const float Z = (float)z*8.0F;
		for (int y = 0; y < MAP_WIDTH; ++y) {
			const float Y = (float)y*8.0F;
			for (int x = 0; x < MAP_HEIGHT; ++x) {
				const float X = (float)x*8.0F;
				const float cell_noise = strataNoise.GetCellular(X, Y, Z);
				const float biome_ramp = (cell_noise + 2.0F) / 4.0F;

				int biome_idx = static_cast<int>(biome_ramp * (float)numStrata);
				++std::get<0>(strata.counts[biome_idx]);
				std::get<1>(strata.counts[biome_idx]) += x;
				std::get<2>(strata.counts[biome_idx]) += y;
				std::get<3>(strata.counts[biome_idx]) += z;
				const int map_idx = getIdx({ x, y, z });
				strata.strata_map[map_idx] = biome_idx;
			}
		}
	}

	// Find a better way to distribute the layers with metamorhpic being lowest ~~ Also add gems?

	int count_used = 0;
	for (int i = 0; i < numStrata; ++i) {
		if (std::get<0>(strata.counts[i]) > 0) {
			++count_used;
			std::get<1>(strata.counts[i]) /= std::get<0>(strata.counts[i]);
			std::get<2>(strata.counts[i]) /= std::get<0>(strata.counts[i]);
			std::get<3>(strata.counts[i]) /= std::get<0>(strata.counts[i]);

			auto & n = std::get<0>(strata.counts[i]);
			auto & x = std::get<1>(strata.counts[i]);
			auto & y = std::get<2>(strata.counts[i]);
			auto & z = std::get<3>(strata.counts[i]);

			const uint8_t altitude_at_center = heightMap[(y * MAP_WIDTH) + x] + MAP_DEPTH / 2;
			
			if (z > altitude_at_center - (1 + rng.getInt(1, 4))) {
				// Soil
				int roll = rng.getInt(1, 100);
				if (100) {						 // Replace once sands
					const std::size_t soil_idx = rng.getInt(1, soils.size()) - 1;
					//std::cout << material_name(soils[soil_idx]) << "\n";
					strata.material_idx[i] = soils[soil_idx];
				}
				else {
					const std::size_t sand_idx = rng.getInt(1, sands.size()) - 1;
					//std::cout << material_name(sands[sand_idx]) << "\n";
					strata.material_idx[i] = sands[sand_idx];
				}
			}
			else if (z > (altitude_at_center - 10) / 2)
			{
				// Sedimentary
				const std::size_t sed_idx = rng.getInt(1, sedimintaries.size()) - 1;
				//std::cout << material_name(sedimintaries[sed_idx]) << "\n";
				strata.material_idx[i] = sedimintaries[sed_idx];
			}
			else if (z > (altitude_at_center - 20) / 2)
			{
				// Igneous
				const std::size_t ig_idx = rng.getInt(1, igneous.size()) - 1;
				//std::cout << material_name(igneous[ig_idx]) << "\n";
				strata.material_idx[i] = igneous[ig_idx];
			}
			else 
			{
				// Metamorphic
				const std::size_t meta_idx = rng.getInt(1, metamorphics.size()) - 1;
				strata.material_idx[i] = metamorphics[meta_idx];
			}
		}
		else {
			//std::cout << "Warning - we didn't think this strata was in use!\n";
			strata.material_idx[i] = 1;
		}
	}
	//std::cout << count_used << " strata detected, " << numStrata - count_used << " unused.\n";

	return strata;
	
}

void layRock(std::vector<uint8_t> heightMap, Strata & strata, TCODRandom & rng)
{

	for (int i = 0; i < MAP_WIDTH; ++i)
		for (int j = 0; j < MAP_HEIGHT; ++j)
		{
			int z = 1;
			const int cell_idx = (j * MAP_WIDTH) + i;
			int alt = heightMap[cell_idx];

			// Add the Rock/Ore's layer
			while (z < std::min(alt + 12, MAP_DEPTH - 15))
			{
				const int idx = getIdx({ i, j, z });
				std::size_t matIdx;
				region::setTileType(idx, TileTypes::SOLID);

				if (idx < strata.strata_map.size())
				{
					const int stratIdx = strata.strata_map[idx];
					if (stratIdx < strata.material_idx.size())
						matIdx = strata.material_idx[stratIdx];

					else
					{
						matIdx = 1;
						std::cout << "Warning - exceeded strata_material size\n";
					}
				}
				else
				{
					matIdx = 1;
					std::cout << "Warning - exceeded strata_map size\n";
				}

				region::setMaterial({ i, j, z }, matIdx); 
				++z;
			}

			const int idx = getIdx({ i, j, z });
			region::makeFloor(idx);

			// Set vegitation ~~ sepperate vegitation from materials

			std::string tmp = "grass";
			int rg = rng.getInt(1, 3, 3);
			tmp += rg + '0';
			region::setMaterial({ i, j, z }, getMaterialIdx(tmp));

			++z;

			// Set empty space with remaining space
			while (z < MAP_DEPTH - 1)
			{
				const int idx = getIdx({ i, j, z });
				region::setTileType(idx, TileTypes::EMPTY_SPACE);
				++z;
			}
		}
}