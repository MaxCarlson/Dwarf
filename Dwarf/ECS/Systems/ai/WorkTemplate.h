#pragma once

#include "../../Component.h"
#include "../../World.h"
#include "../../Components/PositionComponent.h"
#include "../../Components/MovementComponent.h"
#include "../../Components/ItemStored.h"
#include "../../Systems/DijkstraSystems/DijkstraMapsHandler.h"

#include "../../Components/Item.h"
#include "../EntityPositionCache.h"
#include "../../Messages/pickup_item_message.h"
#include "../../Components/Sentients/Inventory.h"
#include "Globals\GlobalWorld.h"

template<typename TAG> // Should this hold a refernce to the Entity so we can stop passing it?
class WorkTemplate
{
public:

	inline bool hasCorrectTool(const Entity& e, const int toolType)
	{
		const auto& inv = e.getComponent<Inventory>();
		const auto& tid = inv.inventory[SLOT_TOOL];

		auto& tent = world.getEntity(tid);

		if (!tent.hasComponent<Item>())
			return false;

		return tent.getComponent<Item>().catagory.test(toolType);
	}

	// Find out if we're holding the tool
	// If not find the closest tools and try to path to them
	// if path is not found run CANCEL llambda
	// else run NEED_TO_GRAB llambda
	template<typename CANCEL, typename SUCCESS, typename NEED_TO_GRAB>
	void findTool(const Entity& e, const Coordinates& co, MovementComponent &mov, const int catagory, const CANCEL &cancel, const SUCCESS &success, const NEED_TO_GRAB &grab)
	{
		if (hasCorrectTool(e, catagory))
		{
			success();
			return;
		}

		// Create a sorted map of closest items so we can 
		// try pathing to multiple items before giving up
		// if one path doesn't work out
		std::map<double, size_t> distanceMap = { { 1000.0, 0 } };

		itemHelper.forEachItem([&distanceMap, &co, &catagory](const Entity& i)
		{
			if (i.hasComponent<Claimed>() || !i.getComponent<Item>().catagory.test(catagory))
				continue;

			distanceMap.insert(std::make_pair(get_3D_distance(i.getComponent<PositionComponent>().co, co), i.getId().index));
		});

		const auto& id = distanceMap.begin()->second;

		bool found = false;
		for (const auto& i : distanceMap)
		{
			auto itemCo = world.getEntity(i.second).getComponent<PositionComponent>().co;

			auto path = findPath(co, itemCo);

			if (path->failed)
				continue;

			found = true;
			mov.path = path->path;
			break;
		}

		if (!found)
		{
			cancel();
			return;
		}

		grab(item);
	}

	// Used when we want to pickup a tool or item from a Dijkstra map, or just a known location
	// without knowing the actual ID of the item
	template<typename MSG, typename CANCEL, typename SUCCESS>
	void pickup_tool(const Entity& e, Coordinates co, const int &catagory, std::size_t& out_tool, const CANCEL &cancel, const SUCCESS &success)
	{
		// Entities current tool
		// ( Possibly no tool )
		//out_tool = e.getComponent<Inventory>().inventory[SLOT_TOOL];

		// If entity already has the correct tool, we don't need to look
		// for one
		if (out_tool && world.getEntity(out_tool).getComponent<Item>().catagory.test(catagory))
		{
			success();
			return;
		}

		// Load all Entities at positon from cache 
		auto& entitiesAtPos = positionCache->get_location(co);
			
		// Entity doesn't have correct tool
		// For all Entities at this position
		for (auto& i : entitiesAtPos)
		{
			const auto item = world.getEntity(i);

			auto& pos = item.getComponent<PositionComponent>().co;
			auto* tool = &item.getComponent<Item>();

			// If position cache is wrong, or Entity doesn't have an
			// Item component, skip it
			if (!tool || pos != co)
				continue;

			// Test and make sure it's the proper tool
			if (tool->catagory.test(catagory))
			{
				// Pickup tool if it matches catagory
				// dump old tool in tool slot if it exists
				world.emit(pickup_item_message{SLOT_TOOL , e.getId().index, item.getId().index, out_tool});
				world.emit(MSG{});

				// Set current tool to tool being picked up
				out_tool = item.getId().index;

				success();
				return;
			}
		}
		cancel();
	}

	template<typename CANCEL, typename SUCCESS>
	void followMap(DijkstraMap &map, const Entity& e, Coordinates co, const CANCEL &cancel, const SUCCESS &success)
	{
		const auto dist = map.get(getIdx(co));

		if (dist > MAX_DIJKSTRA_DISTANCE - 1)
			cancel();

		else if (dist < 1)
			success();
		else
		{
			auto& mov = e.getComponent<MovementComponent>();

			// Don't interrupt progress
			if (mov.progress)
				return;
			else
			{
				const auto destination = map.findDestination(co);
				mov.path.emplace_back(destination);
				return;
			}
		}
	}

	template<typename CANCEL, typename SUCCESS>
	void followPath(MovementComponent &mov, Coordinates co, Coordinates dest, const CANCEL &cancel, const SUCCESS &success)
	{
		// Don't interrupt progress
		if (mov.progress)
			return;

		if (co == dest)
		{
			success();
			return;
		}

		// Try to find path again
		if (mov.path.empty())
		{
			auto path = findPath(co, dest);

			if (path->failed)
			{
				cancel();
				return;
			}

			mov.path = path->path; // Need to make mov.path into a unique ptr so we're not copying the paths
			return;
		}
	}

	inline void cancel_work(Entity e)
	{
		e.removeComponent<TAG>();
		e.activate();
		// status marker for idle here?
	}

};