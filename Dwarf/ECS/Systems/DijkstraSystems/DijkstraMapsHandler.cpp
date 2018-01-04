#include "stdafx.h"
#include "DijkstraMapsHandler.h"
#include "PickMapSystem.h"
#include "Designations.h"
#include "Map\Index.h"
#include "../helpers/ItemHelper.h"
#include "../../Components/Claimed.h"
#include "../../Messages/pick_map_changed_message.h"
#include "../../Messages/designate_architecture_message.h"
#include "../../Messages/block_map_changed_message.h"
#include "../../Messages/update_all_maps_message.h"
#include "../../../Engine.h"

DijkstraMap pick_map;
DijkstraMap block_map;
DijkstraMap architecture_map;


DijkstraMapsHandler::~DijkstraMapsHandler()
{
	delete pickMapSystem;
}

void DijkstraMapsHandler::init()
{
	// Create systems
	pickMapSystem = new PickMapSystem;

	// Init maps
	pick_map.init();
	block_map.init();
	architecture_map.init();

	// Add systems to world
	engine->world.addSystem(*pickMapSystem);

	// Subscribe to messages
	subscribe_mbox<update_all_maps_message>();
	subscribe_mbox<pick_map_changed_message>();
	subscribe_mbox<block_map_changed_message>();
	subscribe_mbox<designate_architecture_message>();
}

void DijkstraMapsHandler::update()
{
	each_mbox<update_all_maps_message>([this]( const update_all_maps_message &msg)
	{
		update_pick_map = true;
		update_block_map = true;
		update_architecture = true;
	});
	each_mbox<pick_map_changed_message>([this](const pick_map_changed_message &msg) { update_pick_map = true; });
	each_mbox<block_map_changed_message>([this](const block_map_changed_message & msg) { update_block_map = true; });
	each_mbox<designate_architecture_message>([this](const designate_architecture_message & msg) { update_architecture = true; });

	if (update_pick_map)
	{
		pickMapSystem->update();
		update_pick_map = false;
	}

	if (update_block_map)
	{
		std::unordered_set<int> repeats;
		std::vector<int> targets;

		itemHelper.forEachItem(
			[&targets, &repeats](const auto& e)
			{
				if (e.hasComponent<Item>() && e.getComponent<Item>().tag != "block")
				{
					if (e.hasComponent<Claimed>() || ! e.hasComponent<PositionComponent>())
						return;

					const auto idx = getIdx(e.getComponent<PositionComponent>().co);

					if (repeats.find(idx) == repeats.end())
					{
						repeats.insert(idx);
						targets.emplace_back(idx);
					}
				}
			}
		);

		block_map.update(targets);
		update_block_map = false;
	}

	if (update_architecture)
	{
		std::vector<int> targets;

		for (const auto& a : designations->architecture)
			targets.emplace_back(a.first);

		architecture_map.update(targets);
		update_architecture = false;
	}
}
