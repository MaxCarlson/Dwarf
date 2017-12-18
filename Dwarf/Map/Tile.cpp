#include "Tile.h"
#include "Util.h"
#include <memory>
#include <iostream>
#include "../Raws/Materials.h"
#include "../Raws/Defs/MaterialDef.h"

#include <cereal.hpp>
#include "../cereal/archives/binary.hpp"
#include "../cereal/types/memory.hpp"
#include "../cereal/types/vector.hpp"
#include <fstream>

#include "../Engine.h"



// External map info
int MAP_WIDTH, MAP_HEIGHT, MAP_DEPTH, TOTAL_MAP_TILES;

namespace region
{
	// Creates a 1D Vector of Tile objects used to
	// simulate a 3D area of tiles. Access Tiles through here
	struct Region
	{
		Region()
		{
			//tileMap.resize(TOTAL_MAP_TILES);
			solid.resize(TOTAL_MAP_TILES, 0);
			tileTypes.resize(TOTAL_MAP_TILES);
			tileFlags.resize(TOTAL_MAP_TILES);
			materials.resize(TOTAL_MAP_TILES);
			tileHealth.resize(TOTAL_MAP_TILES, 1);
		}

		// 1D vector of Tiles indexed by 3D formula
		//std::vector<Tile> tileMap; // Remove this and replace with sepperate vectors of properties

		std::vector<char> solid;

		std::vector<uint16_t> tileTypes;

		std::vector<Util::Bitset<Util::BITSET_16>> tileFlags;

		std::vector<std::size_t> materials;
		std::vector<uint16_t> tileHealth;

		//void tileRecalc(const Coordinates co);
		void tileRecalcAll();
		void spot_recalc_paths(const Coordinates co);
		void tilePathing(const Coordinates co);

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(solid);
			archive(tileTypes);
			archive(tileFlags);
			archive(materials);
			archive(tileHealth);
		}
	};


	std::unique_ptr<Region> currentRegion;

	void new_region(int width, int height, int depth)
	{
		MAP_WIDTH = width;
		MAP_HEIGHT = height;
		MAP_DEPTH = depth;
		TOTAL_MAP_TILES = MAP_WIDTH * MAP_HEIGHT * MAP_DEPTH;

		currentRegion = std::make_unique<Region>();
	}

	void save_region(std::string filePath)
	{
		std::string regionPath = filePath + std::string("_region.sav");

		std::ofstream os(regionPath, std::ios::binary);
		cereal::BinaryOutputArchive oarchive(os);

		oarchive(MAP_WIDTH, MAP_HEIGHT, MAP_DEPTH, TOTAL_MAP_TILES);
		currentRegion->serialize(oarchive);
	}

	void load_region(std::string filePath)
	{
		std::string regionPath = filePath + std::string("_region.sav");

		std::ifstream os(regionPath, std::ios::binary);
		cereal::BinaryInputArchive iarchive(os);

		currentRegion = std::make_unique<Region>();

		iarchive(MAP_WIDTH, MAP_HEIGHT, MAP_DEPTH, TOTAL_MAP_TILES);
		currentRegion->serialize(iarchive);
		//tileRecalcAll(); 
		
	}

	bool flag(const Coordinates co, Flag f)
	{
		return currentRegion->tileFlags[getIdx(co)].test(f);
	}

	void setFlag(const Coordinates co, Flag f)
	{
		currentRegion->tileFlags[getIdx(co)].set(f);
	}

	void resetFlag(const Coordinates co, Flag f)
	{
		currentRegion->tileFlags[getIdx(co)].reset(f);
	}

	bool solid(const int idx)
	{
		return currentRegion->solid[idx];
	}

	void setMaterial(const Coordinates co, const std::size_t mat)
	{
		currentRegion->materials[getIdx(co)] = mat;
		auto material = getMaterial(mat);
		currentRegion->tileHealth[getIdx(co)] = material->health;
	}

	int getTileType(const int idx)
	{
		return currentRegion->tileTypes[idx];
	}

	void setTileType(const int idx, const uint8_t type)
	{
		currentRegion->tileTypes[idx] = type;
	}

	std::size_t getTileMaterial(const Coordinates co)
	{
		return currentRegion->materials[getIdx(co)];
	}

	void damageTile(const int idx, const uint16_t dmg)
	{
		if (currentRegion->tileHealth[idx] > dmg)
			currentRegion->tileHealth[idx] -= dmg;
		else
			currentRegion->tileHealth[idx] = 0;
	}

	uint16_t tileHealth(const int idx)
	{
		return currentRegion->tileHealth[idx];
	}

	void spot_recalc_paths(const Coordinates co)
	{
		currentRegion->spot_recalc_paths(co);
	}

	//void tileRecalc(const Coordinates co)
	//{
	//	currentRegion->tileRecalc(co);
	//}

	void tileRecalcAll()
	{
		currentRegion->tileRecalcAll();
	}

	void makeWall(const int idx) // Add material idx variable
	{
		currentRegion->tileFlags[idx].reset(CAN_STAND_HERE);
		currentRegion->solid[idx] = true;
		currentRegion->tileTypes[idx] = TileTypes::WALL;
	}

	void makeEarth(const int idx)
	{
		currentRegion->tileFlags[idx].reset(CAN_STAND_HERE);
		currentRegion->solid[idx] = true;
		currentRegion->tileTypes[idx] = TileTypes::SOLID; // Change to tileType Earth?


		const Coordinates co = idxToCo(idx);
		if (co.z < MAP_DEPTH - 1)
			makeFloor(getIdx({ co.x, co.y, co.z + 1 }));
	}

	void makeRamp(const int idx)
	{		
		currentRegion->tileFlags[idx].set(CAN_STAND_HERE);
		currentRegion->solid[idx] = false;
		currentRegion->tileTypes[idx] = TileTypes::RAMP;

		const Coordinates co = idxToCo(idx);
		setMaterial(co, getTileMaterial(idxToCo(idx)));

		currentRegion->tileFlags[getIdx({ co.x, co.y, co.z + 1 })].set(CAN_STAND_HERE); // Remove this eventually?
	}

	void makeFloor(const int idx)
	{
		currentRegion->tileFlags[idx].set(CAN_STAND_HERE);
		currentRegion->solid[idx] = false;
		currentRegion->tileTypes[idx] = TileTypes::FLOOR;
	}

	void makeEmptySpace(const int idx)
	{
		currentRegion->tileFlags[idx].reset(CAN_STAND_HERE);
		currentRegion->solid[idx] = false;
		currentRegion->tileTypes[idx] = TileTypes::EMPTY_SPACE;
	}


	int get_rough_distance(Coordinates loc, Coordinates dest)
	{
		int x = dest.x - loc.x;
		int y = dest.y - loc.y;
		int z = dest.z - loc.z;
		return (x * x + y * y + z * z);
	}

	/* Get rid of these, replace completely with flags and std::vector<uint8/16_t> tileTypes */
	//Tile & tileAt(Coordinates co)
	//{
	//	return currentRegion->tileMap[TILE_ARRAY_LOOKUP];
	//}

	//Tile & tileAt(int exactPos)
	//{
	//	return currentRegion->tileMap[exactPos];
	//}

	bool boundsCheck(Coordinates co)
	{
		return(co.x < MAP_WIDTH && co.x >= 0 && co.y < MAP_HEIGHT && co.y >= 0 && co.z < MAP_DEPTH && co.z >= 0);
	}

	/*
	bool canWalk(Coordinates co)
	{
		return boundsCheck(co) && (getProperty<Tile::FLOOR>(co) && !(getProperty<Tile::WALL>(co) | getProperty<Tile::OBSTRUCTED>(co)));
	}

	bool canPass(Coordinates co)
	{
		return boundsCheck(co) && !(getProperty<Tile::WALL>(co) | getProperty<Tile::OBSTRUCTED>(co));
	}

	bool isEmptySpace(Coordinates co)
	{
		return !(getProperty<Tile::FLOOR>(co) | getProperty<Tile::WALL>(co) | getProperty<Tile::OBSTRUCTED>(co));
	}
	*/
	/* END */

	//void Region::tileRecalc(const Coordinates co)
	//{
	//}

	void Region::tileRecalcAll()
	{
		for (int z = 1; z < MAP_DEPTH - 1; ++z)
			for (int y = 0; y < MAP_HEIGHT - 1; ++y)
				for (int x = 0; x < MAP_WIDTH - 1; ++x)
					tilePathing({ x, y, z });
	}

	void Region::spot_recalc_paths(const Coordinates co)
	{
		tilePathing(CO_NORTH);
		tilePathing(CO_SOUTH);
		tilePathing(CO_WEST);
		tilePathing(CO_EAST);
		tilePathing(CO_NORTH_W);
		tilePathing(CO_NORTH_E);
		tilePathing(CO_SOUTH_W);
		tilePathing(CO_SOUTH_E);
		tilePathing(CO_UP);
		tilePathing(CO_DOWN);
		tilePathing(co);
	}

	void Region::tilePathing(const Coordinates co)
	{
		const auto idx = getIdx(co);

		tileFlags[idx].reset(CAN_GO_NORTH); // Add a function to zero a flags bitset
		tileFlags[idx].reset(CAN_GO_SOUTH);
		tileFlags[idx].reset(CAN_GO_EAST);
		tileFlags[idx].reset(CAN_GO_WEST);
		tileFlags[idx].reset(CAN_GO_NORTH_W);
		tileFlags[idx].reset(CAN_GO_NORTH_E);
		tileFlags[idx].reset(CAN_GO_SOUTH_W);
		tileFlags[idx].reset(CAN_GO_SOUTH_E);
		tileFlags[idx].reset(CAN_GO_DOWN);
		tileFlags[idx].reset(CAN_GO_UP);
		
		if (solid[idx] || !tileFlags[getIdx(co)].test(CAN_STAND_HERE))
		{
		}
		else
		{
			if (co.y > 0              && tileFlags[getIdx(CO_NORTH)].test(CAN_STAND_HERE)) tileFlags[idx].set(CAN_GO_NORTH);
			if (co.y < MAP_HEIGHT - 1 && tileFlags[getIdx(CO_SOUTH)].test(CAN_STAND_HERE)) tileFlags[idx].set(CAN_GO_SOUTH);

			if (co.x > 0             && tileFlags[getIdx(CO_WEST)].test(CAN_STAND_HERE)) tileFlags[idx].set(CAN_GO_WEST);
			if (co.x < MAP_WIDTH - 1 && tileFlags[getIdx(CO_EAST)].test(CAN_STAND_HERE)) tileFlags[idx].set(CAN_GO_EAST);

			if (co.y > 0 && co.x > 0             && tileFlags[getIdx(CO_NORTH_W)].test(CAN_STAND_HERE)) tileFlags[idx].set(CAN_GO_NORTH_W);
			if (co.y > 0 && co.x < MAP_WIDTH - 1 && tileFlags[getIdx(CO_NORTH_E)].test(CAN_STAND_HERE)) tileFlags[idx].set(CAN_GO_NORTH_E);

			if (co.y < MAP_HEIGHT - 1 && co.x > 0             && tileFlags[getIdx(CO_SOUTH_W)].test(CAN_STAND_HERE)) tileFlags[idx].set(CAN_GO_SOUTH_W);
			if (co.y < MAP_HEIGHT - 1 && co.x < MAP_WIDTH - 1 && tileFlags[getIdx(CO_SOUTH_E)].test(CAN_STAND_HERE)) tileFlags[idx].set(CAN_GO_SOUTH_E);

			if (co.z < MAP_DEPTH - 1 && tileFlags[getIdx(CO_UP  )].test(CAN_STAND_HERE) && tileTypes[idx] == TileTypes::RAMP) tileFlags[idx].set(CAN_GO_UP);
			if (co.z > 0             && tileFlags[getIdx(CO_DOWN)].test(CAN_STAND_HERE) && tileTypes[getIdx(CO_DOWN)]) tileFlags[idx].set(CAN_GO_DOWN);
		}
	}
}


