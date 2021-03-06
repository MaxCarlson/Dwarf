#include "../stdafx.h"
#include "Tile.h"
#include "Util.h"
#include "Raws\ReadPlants.h"
#include "Raws\Defs\PlantDef.h"
#include "../Raws/Materials.h"
#include "Globals\TexDefines.h"
#include "../Raws/Defs/MaterialDef.h"
#include "../cereal/archives/binary.hpp"
#include "../cereal/types/memory.hpp"
#include "../cereal/types/vector.hpp"
#include "../Drawing/vchar.h"
#include <cereal.hpp>
#include <cereal\types\unordered_set.hpp>

// External map info
int MAP_WIDTH, MAP_HEIGHT, MAP_DEPTH, TOTAL_MAP_TILES;

static int farmingChars[] = { 571, 572, 573, 573 };

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
			stockpileSquares.reserve(200);

			plantType.resize(TOTAL_MAP_TILES, 0);
			plantHealth.resize(TOTAL_MAP_TILES, 0);
			plantTicker.resize(TOTAL_MAP_TILES, 0);
			plantLifeCycle.resize(TOTAL_MAP_TILES, 0);
		}

		std::vector<char> solid;
		//std::vector<char> aboveGround;

		std::vector<uint16_t> tileTypes;

		std::vector<Util::Bitset<Util::BITSET_16>> tileFlags;

		std::vector<std::size_t> materials;
		std::vector<uint16_t> tileHealth;

		std::vector<vchar> renderCache;

		boost::container::flat_map<int, std::size_t> stockpileSquares;

		// This should possible be changed to an unordered map, indexed by Coordinates idx? 
		// As is we have to search through every tile to delete a tree
		std::vector<std::size_t> treeIds;

		// Tree health indexed by tree id
		std::unordered_map<int, int> treeHps;

		// plant info containers
		std::vector<size_t>   plantType;		
		std::vector<uint8_t>  plantHealth;
		std::vector<uint16_t> plantTicker;
		std::vector<uint8_t>  plantLifeCycle;

		// Holds idx of all farm plots so we can 
		// override their graphics with plots
		// all other info about plants is stored as it normally would be
		std::unordered_set<int> farmPlots;

		size_t biomeIdx = 0;
		int nextTreeId = 0;
		
		void tileRecalcAll();
		void spot_recalc_paths(const Coordinates co);
		void tilePathing(const Coordinates co);
		void tileRecalc(const Coordinates co);
		void tileCalcRender(Coordinates co);
		std::pair<vchar, int> getRenderTile(const Coordinates co);

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(solid);
			//archive(aboveGround);
			archive(tileTypes);
			archive(tileFlags);
			archive(materials);
			archive(tileHealth);
			//archive(renderCache);
			archive(stockpileSquares);
			archive(treeIds);
			archive(treeHps);
			archive(nextTreeId);
			archive(biomeIdx);
			archive(plantType, plantHealth, plantTicker, plantLifeCycle);
			archive(farmPlots);
		}
	};


	std::unique_ptr<Region> currentRegion;

	void new_region(int width, int height, int depth, size_t biomeIdx)
	{
		MAP_WIDTH = width;
		MAP_HEIGHT = height;
		MAP_DEPTH = depth;
		TOTAL_MAP_TILES = MAP_WIDTH * MAP_HEIGHT * MAP_DEPTH;

		currentRegion = std::make_unique<Region>();
		currentRegion->biomeIdx = biomeIdx;
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

		iarchive(MAP_WIDTH, MAP_HEIGHT, MAP_DEPTH, TOTAL_MAP_TILES);
		currentRegion = std::make_unique<Region>();

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
		currentRegion->solid[idx] = 1;
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

	std::pair<vchar, int> getRenderTile(const Coordinates co)
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

	int treeId(const int idx)
	{
		return currentRegion->treeIds[idx];
	}

	void setTreeHealth(const int id, const int hp)
	{
		currentRegion->treeHps[id] = hp;
	}

	int treeHealth(const int id)
	{
		return currentRegion->treeHps[id];
	}

	void damageTree(const int id, const int dmg)
	{
		currentRegion->treeHps[id] -= dmg;

		if (currentRegion->treeHps[id] < 0)
			currentRegion->treeHps[id] = 0;
	}

	void deleteTree(const int id)
	{
		currentRegion->treeHps.erase(id);

		for (auto& t : currentRegion->treeIds) // Possibly change data structre or search method if this becomes a resource drain, searching through all tiles
			if (t == id)
				t = 0;
	}

	size_t plantType(const int idx)
	{
		return currentRegion->plantType[idx];
	}

	uint8_t plantHealth(const int idx)
	{
		return currentRegion->plantHealth[idx];
	}

	uint16_t plantTicker(const int idx)
	{
		return currentRegion->plantTicker[idx];
	}

	uint8_t plantLifeCycle(const int idx)
	{
		return currentRegion->plantLifeCycle[idx];
	}

	void setPlantType(const int idx, const uint8_t type)
	{
		currentRegion->plantType[idx] = type;
	}

	void setPlantHealth(const int idx, const uint8_t hp)
	{
		currentRegion->plantHealth[idx] = hp;
	}

	void setPlantTicker(const int idx, const uint16_t tick)
	{
		currentRegion->plantTicker[idx] = tick;
	}

	void setPlantLifecycle(const int idx, const uint8_t lc)
	{
		currentRegion->plantLifeCycle[idx] = lc;
	}

	void damagePlant(const int idx, const int dmg)
	{
		if (currentRegion->plantHealth[idx] > dmg)
			currentRegion->plantHealth[idx] -= dmg;
		else
		{
			currentRegion->plantHealth[idx] = 0; // Should we also delete plant here or not?
		}
	}

	void setFarmPlot(const int idx)
	{
		currentRegion->farmPlots.emplace(idx);
	}

	void deleteFarmPlot(const int idx)
	{
		currentRegion->farmPlots.erase(idx);
	}

	void forFarmPlots(std::function<void(const int)> func)
	{
		for (const int plot : currentRegion->farmPlots)
			func(plot);
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
		currentRegion->tileFlags[idx].set(Flag::CONSTRUCTION);
		currentRegion->tileFlags[idx].reset(CAN_STAND_HERE);
		currentRegion->solid[idx] = true;
		currentRegion->tileTypes[idx] = TileTypes::WALL;
		currentRegion->plantType[idx] = 0;
	}

	void makeEarth(const int idx)
	{
		currentRegion->tileFlags[idx].reset(CAN_STAND_HERE);
		currentRegion->solid[idx] = true;
		currentRegion->tileTypes[idx] = TileTypes::SOLID; // Change to tileType Earth?
		currentRegion->plantType[idx] = 0;
	}

	void makeRamp(const int idx)
	{		
		currentRegion->tileFlags[idx].set(CAN_STAND_HERE);
		currentRegion->solid[idx] = false;
		currentRegion->tileTypes[idx] = TileTypes::RAMP;
		currentRegion->plantType[idx] = 0;

		const Coordinates co = idxToCo(idx);

		currentRegion->tileFlags[getIdx({ co.x, co.y, co.z + 1 })].set(CAN_STAND_HERE); // Remove this eventually?
	}

	void makeUpStair(const int idx)
	{
		currentRegion->tileFlags[idx].set(CAN_STAND_HERE);
		currentRegion->solid[idx] = false;
		currentRegion->tileTypes[idx] = TileTypes::UP_STAIRS;
		currentRegion->plantType[idx] = 0;
	}

	void makeDownStair(const int idx)
	{
		currentRegion->tileFlags[idx].set(CAN_STAND_HERE);
		currentRegion->solid[idx] = false;
		currentRegion->tileTypes[idx] = TileTypes::DOWN_STAIRS;
		currentRegion->plantType[idx] = 0;
	}

	void makeUpDownStair(const int idx)
	{
		currentRegion->tileFlags[idx].set(CAN_STAND_HERE);
		currentRegion->solid[idx] = false;
		currentRegion->tileTypes[idx] = TileTypes::UP_DOWN_STAIRS;
		currentRegion->plantType[idx] = 0;
	}

	void makeFloor(const int idx)
	{
		currentRegion->tileFlags[idx].set(CAN_STAND_HERE);
		currentRegion->solid[idx] = false;
		currentRegion->tileTypes[idx] = TileTypes::FLOOR;
		currentRegion->plantType[idx] = 0;
	}

	void makeEmptySpace(const int idx)
	{
		currentRegion->tileFlags[idx].reset(CAN_STAND_HERE);
		currentRegion->solid[idx] = false;
		currentRegion->tileTypes[idx] = TileTypes::EMPTY_SPACE;
		currentRegion->plantType[idx] = 0;
	}

	int groundLevel(const int x, const int y)
	{
		for (int z = MAP_DEPTH - 1; z > 0; --z)
		{
			auto tt = currentRegion->tileTypes[getIdx({ x, y, z })];
			if (tt == TileTypes::FLOOR) // TODO: This will likely need to be changed when walls are introduced
				return z;
			else if (tt == TileTypes::SOLID)
				return z + 1;
		}
		return 1;
	}

	bool boundsCheck(Coordinates co)
	{
		return(co.x < MAP_WIDTH - 1 && co.x >= 0 && co.y < MAP_HEIGHT - 1 && co.y >= 0 && co.z < MAP_DEPTH - 1 && co.z >= 0);
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

		tileFlags[idx].reset(CAN_GO_ALL_DIRECTIONS);
		
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

			if (co.z < MAP_DEPTH - 2 && tileFlags[getIdx(CO_UP  )].test(CAN_STAND_HERE) 
				&& ( tileTypes[idx] == TileTypes::RAMP 
				|| (tileTypes[getIdx(CO_UP)] == TileTypes::DOWN_STAIRS && ( tileTypes[idx] == TileTypes::UP_STAIRS || tileTypes[idx] == TileTypes::UP_DOWN_STAIRS)))) tileFlags[idx].set(CAN_GO_UP);


			if (co.z > 0 
				&&    tileFlags[getIdx(CO_DOWN)].test(CAN_STAND_HERE)
				&& (  tileTypes[getIdx(CO_DOWN)] == TileTypes::RAMP 
				|| (( tileTypes[getIdx(CO_DOWN)] == TileTypes::UP_STAIRS || tileTypes[getIdx(CO_DOWN)] == TileTypes::UP_DOWN_STAIRS) 
			    && (  tileTypes[idx] == TileTypes::DOWN_STAIRS || tileTypes[idx] == TileTypes::UP_DOWN_STAIRS)))) tileFlags[idx].set(CAN_GO_DOWN);
		}
	}

	void Region::tileRecalc(const Coordinates co)
	{
		tileCalcRender(co);
		tilePathing(co);
	}

	void Region::tileCalcRender(Coordinates co)
	{
		int idx		= getIdx(co);
		int ch		= 0;
		uint32_t fg = color_from_name("black");
		uint32_t bg = color_from_name("black");

		if (!tileFlags[idx].test(DISCOVERED))
			goto End;

		switch (tileTypes[idx])
		{
		case TileTypes::EMPTY_SPACE:
			break;

		case TileTypes::SOLID:
		{
			const auto& mat = getMaterial(materials[idx]); // Switch rendering to use vchars!!! Add debugging tools in here if need be
			fg = color_from_name(mat->color.c_str());
			ch = mat->charCode;
		}
			break;

		case TileTypes::FLOOR:
		{
			const auto& mat = getMaterial(materials[idx]); // Switch rendering to use vchars!!! Add debugging tools in here if need be
			fg = color_from_name(mat->color.c_str());
			ch = mat->floorCode;

			if (stockpileId(idx) > 0) // Stockpiles override plants? Should they delete plants as well?
			{
				fg = color_from_name("dark grey"); 
				ch = TEX_STOCKPILE;
			}

			else if (farmPlots.find(idx) != farmPlots.end() || plantType[idx] > 0)
			{
				const auto plant = getPlantDef(plantType[idx]);
				const auto lifecycle = plantLifeCycle[idx];

				if (farmPlots.find(idx) == farmPlots.end())
					ch = plant->chars[lifecycle].c;
				else
				{
					ch = farmingChars[lifecycle];

					if (lifecycle > 1 && plant->harvestsTo[lifecycle] == "none")
						ch = farmingChars[0];
				}
					

				fg = plant->chars[lifecycle].fg; 
			}
		}
			break;

		case TileTypes::WALL: // Not implemented (need all the corners, etc) yet
		{
			const auto& mat = getMaterial(materials[idx]);
			ch = 205;
			fg = color_from_name(mat->color.c_str());
		}
			break;

		case TileTypes::RAMP:
		{
			const auto& mat = getMaterial(materials[idx]); // Switch rendering to use vchars!!! Add debugging tools in here if need be
			fg = color_from_name(mat->color.c_str());
			ch = 30;
		}
			break;

		case TileTypes::TREE_TRUNK:
			ch = 10;
			fg = color_from_name("brown");
			break;

		case TileTypes::TREE_LEAF:
			ch = 244;
			fg = color_from_name("green");
			break;
		}

		End:
		renderCache[idx] = vchar{ ch, fg, bg };
	}

	std::pair<vchar, int> Region::getRenderTile(const Coordinates co)
	{
		// Loop through z levels until hitting ground and return first visible tile
		int z;
		for (z = co.z; z > 0; --z)
		{
			const auto idx = getIdx({ co.x, co.y, z });
			const auto& tt = tileTypes[idx];

			if (tt != TileTypes::EMPTY_SPACE)
				return { renderCache[idx], z };
		}
		return { vchar{ 0, color_from_name("black"), color_from_name("black") }, z };
	}
}


