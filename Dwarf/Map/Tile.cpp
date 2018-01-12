#include "../stdafx.h"
#include "Tile.h"
#include "Util.h"
#include "../Raws/Materials.h"
#include "../Raws/Defs/MaterialDef.h"
#include "../cereal/archives/binary.hpp"
#include "../cereal/types/memory.hpp"
#include "../cereal/types/vector.hpp"
#include "../Drawing/vchar.h"
#include "../Engine.h"
#include <cereal.hpp>

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
			solid.resize(TOTAL_MAP_TILES, 0);
			//aboveGround.resize(TOTAL_MAP_TILES, 0);
			tileTypes.resize(TOTAL_MAP_TILES);
			tileFlags.resize(TOTAL_MAP_TILES);
			materials.resize(TOTAL_MAP_TILES);
			tileHealth.resize(TOTAL_MAP_TILES, 1);
			treeIds.resize(TOTAL_MAP_TILES, 0);
			renderCache.resize(TOTAL_MAP_TILES);
			stockpileSquares.reserve(100);
		}

		std::vector<char> solid;
		//std::vector<char> aboveGround;

		std::vector<uint16_t> tileTypes;

		std::vector<Util::Bitset<Util::BITSET_16>> tileFlags;

		std::vector<std::size_t> materials;
		std::vector<uint16_t> tileHealth;

		std::vector<std::size_t> treeIds;
		std::vector<vchar> renderCache;

		boost::container::flat_map<int, std::size_t> stockpileSquares;

		int nextTreeId = 0;
		
		void tileRecalcAll();
		void spot_recalc_paths(const Coordinates co);
		void tilePathing(const Coordinates co);
		void tileRecalc(const Coordinates co);
		void tileCalcRender(Coordinates co);
		vchar getRenderTile(const Coordinates co);

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(solid);
			//archive(aboveGround);
			archive(tileTypes);
			archive(tileFlags);
			archive(materials);
			archive(tileHealth);
			archive(treeIds);
			archive(renderCache);
			archive(stockpileSquares);
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
		tile_recalc_all();
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

	void setSolid(const int idx)
	{
		currentRegion->solid[idx] = true;
	}

	bool solid(const int idx)
	{
		return currentRegion->solid[idx];
	}
	/*
	bool aboveGround(const int idx)
	{
		return currentRegion->aboveGround[idx];
	}
	*/
	vchar & renderCache(const int idx)
	{
		return currentRegion->renderCache[idx];
	}

	vchar getRenderTile(const Coordinates co)
	{
		return currentRegion->getRenderTile(co);
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

	void setStockpileId(const int idx, const std::size_t id)
	{
		currentRegion->stockpileSquares[idx] = id;
	}

	std::size_t stockpileId(const int idx)
	{
		if(currentRegion->stockpileSquares[idx])
			return currentRegion->stockpileSquares[idx];

		return 0;
	}

	void forStockpileSquares(const std::function<void(int, std::size_t)>& func)
	{
		for (auto it = currentRegion->stockpileSquares.cbegin(); it != currentRegion->stockpileSquares.cend(); ++it)
		{
			func(it->first, it->second);
		}
	}

	int nextTreeId()
	{
		return ++currentRegion->nextTreeId;
	}

	void setTreeId(const int idx, const int id)
	{
		currentRegion->treeIds[idx] = id;
	}

	void spot_recalc_paths(const Coordinates co)
	{
		currentRegion->spot_recalc_paths(co);
	}

	void tile_recalc_all()
	{
		currentRegion->tileRecalcAll();
	}

	void tile_recalc(const Coordinates co)
	{
		currentRegion->tileRecalc(co);
	}

	void tile_calc_render(const Coordinates co)
	{
		currentRegion->tileCalcRender(co);
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

	int groundLevel(const int x, const int y)
	{
		for (int z = MAP_DEPTH - 1; z > 0; --z)
		{
			if (currentRegion->tileTypes[getIdx({ x, y, z })] == TileTypes::FLOOR)
				return z;
		}
		return 0;
	}

	double get_3D_distance(Coordinates loc, Coordinates dest)
	{
		double x = dest.x - loc.x;
		double y = dest.y - loc.y;
		double z = dest.z - loc.z;
		return (x * x + y * y + z * z);
	}

	double get_2D_distance(Coordinates loc, Coordinates dest)
	{
		double x = dest.x - loc.x;
		double y = dest.y - loc.y;
		return (x * x + y * y);
	}

	bool boundsCheck(Coordinates co)
	{
		return(co.x < MAP_WIDTH && co.x >= 0 && co.y < MAP_HEIGHT && co.y >= 0 && co.z < MAP_DEPTH && co.z >= 0);
	}

	void Region::tileRecalcAll()
	{
		for (int z = 1; z < MAP_DEPTH - 1; ++z)
			for (int y = 0; y < MAP_HEIGHT - 1; ++y)
				for (int x = 0; x < MAP_WIDTH - 1; ++x)
				{
					tilePathing({ x, y, z });
					tileCalcRender({ x, y, z });
				}
					
		/*
		for (int z = 1; z < MAP_DEPTH - 1; ++z)
			for (int y = 0; y < MAP_HEIGHT - 1; ++y)
				for (int x = 0; x < MAP_WIDTH - 1; ++x)
					tileRecalc({ x, y, z });
		*/
	}

	void Region::spot_recalc_paths(const Coordinates co) // Rename this tile_spot_recalc
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

		tileRecalc(co);
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

	void Region::tileRecalc(const Coordinates co)
	{
		tileCalcRender(co);
		const int idx = getIdx(co);

		/*
		if (co.z == MAP_DEPTH - 1)
			aboveGround[idx] = true;

		else
		{
			bool ug = false;
			for (int z = MAP_DEPTH - 1; z > 0; --z)
				if (tileTypes[getIdx({ co.x, co.y, z })] == TileTypes::SOLID)
				{
					ug = true;
					break;
				}
					

			aboveGround[idx] = !ug;
		}
		*/
		//tilePathing(co);
	}

	void Region::tileCalcRender(Coordinates co)
	{
		const int idx = getIdx(co);
		int ch = 0;
		std::string fg = "black";
		std::string bg = "black";

		switch (tileTypes[idx])
		{
		case TileTypes::EMPTY_SPACE:
			break;

		case TileTypes::SOLID:
		{
			const auto& mat = getMaterial(materials[idx]); // Switch rendering to use vchars!!! Add debugging tools in here if need be
			fg = mat->color;
			ch = mat->charCode;
		}
			break;

		case TileTypes::FLOOR:
		{
			const auto& mat = getMaterial(materials[idx]); // Switch rendering to use vchars!!! Add debugging tools in here if need be
			fg = mat->color;
			ch = mat->floorCode;
		}
			break;

		case TileTypes::WALL: // Not implemented yet
		{
			const auto& mat = getMaterial(materials[idx]);
			ch = 205;
			fg = mat->color;
		}
			break;

		case TileTypes::RAMP:
		{
			const auto& mat = getMaterial(materials[idx]); // Switch rendering to use vchars!!! Add debugging tools in here if need be
			fg = mat->color;
			ch = 30;
		}
			break;

		case TileTypes::TREE_TRUNK:
			ch = 10;
			fg = "brown";
			break;

		case TileTypes::TREE_LEAF:
			ch = 244;
			fg = "green";
			break;
		}

		renderCache[idx] = vchar{ ch, color_from_name(fg.c_str()), color_from_name(bg.c_str()) };
	}
	vchar Region::getRenderTile(const Coordinates co)
	{
		// Loop through z levels until hitting ground and return first visible tile
		for (int z = co.z; z > 0; --z)
		{
			const auto idx = getIdx({ co.x, co.y, z });
			const auto& tt = tileTypes[idx];

			if (tt != TileTypes::EMPTY_SPACE)
				return renderCache[idx];
		}
		return vchar{ 0, color_from_name("black"), color_from_name("black") };
	}
}


