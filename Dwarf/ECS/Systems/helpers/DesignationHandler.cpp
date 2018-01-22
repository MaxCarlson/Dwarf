#include "../stdafx.h"
#include "DesignationHandler.h"
#include "../../Messages/designation_message.h"
#include "../Map/Tile.h"
#include "../Designations.h"
#include "../ECS/Messages/recalculate_mining_message.h"
#include "../ECS/Messages/pick_map_changed_message.h"
#include "../ECS/Messages/axemap_changed_message.h"
#include "../ECS/Messages/harvest_map_changed_message.h"
#include "Raws\ReadPlants.h"
#include "Raws\Defs\PlantDef.h"

void DesignationHandler::init()
{
	subscribe<designation_message>([this](designation_message &msg)
	{
		designate(msg);
	});
}

void DesignationHandler::designate(const designation_message & msg) // Add in checks to make sure improper areas aren't designated
{
	auto co1 = idxToCo(msg.desIdx.first);							
	auto co2 = idxToCo(msg.desIdx.second); 
	const auto type = msg.type;

	// Don't allow multi dimensional designations ~~ Possibly allow eventually
	if (co1.z != co2.z)
		return;

	// Flip variables if needed so we can enumerate through loop easily
	if (co1.x > co2.x)
	{
		int tmp = co1.x;
		co1.x = co2.x;
		co2.x = tmp;
	}
	if (co1.y > co2.y)
	{
		int tmp = co1.y;
		co1.y = co2.y;
		co2.y = tmp;
	}

	// Handle chopping designation
	if (msg.type == designation_message::CUT_TREES)
	{
		designateChopping(co1, co2);
	}

	else if (msg.type == designation_message::HARVEST)
	{
		designateHarvest(co1, co2);
	}

	// Handle a mining like designation
	else
	{
		designateMining(type, co1, co2);
	}
}

void DesignationHandler::designateMining(const int type, const Coordinates co1, const Coordinates co2)
{
	for (int x = co1.x; x <= co2.x; ++x)
		for (int y = co1.y; y <= co2.y; ++y)
		{
			designations->mining[getIdx({ x, y, co1.z })] = type;
		}

	emit(recalculate_mining_message{});
	emit(pick_map_changed_message{}); // Is this needed?
}

inline int isTreeAccessible(const Coordinates co) 
{
	int id = region::treeId(getIdx(co));

	return id;
}

inline Coordinates findTreeBottom(Coordinates co, const int id)
{
	for (int z = co.z - 1; z > 0; --z)
	{
		int zid = region::treeId(getIdx({ co.x, co.y, z }));

		if (zid == id)
			co = { co.x, co.y, z };
	}

	return co;

	// Possibly incorporate this into tree bottom finding, if we encounter instances where unreachable tree parts are designated
	// even though they are the lowest tree point
	/*
	// Make sure we only designate a tree tile that is ground level for that tree
	// Is there a better way to do this?

	if (   getTileType(getIdx(CO_NORTH))   == TileTypes::FLOOR
		|| getTileType(getIdx(CO_NORTH_W)) == TileTypes::FLOOR
		|| getTileType(getIdx(CO_NORTH_E)) == TileTypes::FLOOR
		|| getTileType(getIdx(CO_SOUTH))   == TileTypes::FLOOR
		|| getTileType(getIdx(CO_SOUTH_W)) == TileTypes::FLOOR
		|| getTileType(getIdx(CO_SOUTH_E)) == TileTypes::FLOOR
		|| getTileType(getIdx(CO_WEST))    == TileTypes::FLOOR
		|| getTileType(getIdx(CO_EAST))    == TileTypes::FLOOR)
	{
	}
	*/
}

void DesignationHandler::designateChopping(const Coordinates co1, const Coordinates co2)
{
	for (int x = co1.x; x <= co2.x; ++x)
		for (int y = co1.y; y <= co2.y; ++y)
		{
			int tid = isTreeAccessible({ x, y, co1.z });

			// If there's a tree, 
			// Find it's lowest trunk that's accessible
			if (tid)
			{
				designations->woodcutting[tid] = findTreeBottom({ x, y, co1.z }, tid);
			}
		}
	emit(axemap_changed_message{});
}

void DesignationHandler::designateHarvest(const Coordinates co1, const Coordinates co2)
{
	for (int x = co1.x; x <= co2.x; ++x)
		for (int y = co1.y; y <= co2.y; ++y)
		{
			const int idx = getIdx({ x, y, co1.z });
			const int t = region::plantType(idx);

			if (t > 0)
			{
				const auto plant = getPlantDef(t);

				if (plant && plant->harvestsTo[region::plantLifeCycle(idx)] != "none")
				{
					designations->harvest.emplace_back(std::make_pair(true, Coordinates{ x, y, co1.z }));
				}
			}
		}
	emit(harvest_map_changed_message{});
}
