#include "stdafx.h"
#include "DijkstraMapsHandler.h"
#include "Designations.h"
#include "Map\Index.h"
#include "../helpers/ItemHelper.h"
#include "../../../Raws/Defs/ItemDefs.h"
#include "../../Components/Claimed.h"
#include "../../Messages/pick_map_changed_message.h"
#include "../../Messages/axemap_changed_message.h"
#include "../../Messages/designate_architecture_message.h"
#include "../../Messages/block_map_changed_message.h"
#include "../../Messages/update_all_maps_message.h"
#include "../../../Engine.h"

DijkstraMap pick_map;
DijkstraMap block_map;
DijkstraMap architecture_map;
DijkstraMap axe_map;


DijkstraMapsHandler::~DijkstraMapsHandler()
{
}

void DijkstraMapsHandler::init()
{
	// Init maps
	pick_map.init();
	axe_map.init();
	block_map.init();
	architecture_map.init();
	
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
		update_axe_map = true;
		update_block_map = true;
		update_architecture = true;
	});
	each_mbox<pick_map_changed_message>([this](const pick_map_changed_message &msg) { update_pick_map = true; });
	each_mbox<block_map_changed_message>([this](const block_map_changed_message & msg) { update_block_map = true; });
	each_mbox<designate_architecture_message>([this](const designate_architecture_message & msg) { update_architecture = true; });

	if (update_pick_map)
	{
		std::vector<int> targets;

		itemHelper.forEachItem([&targets](auto& e) {
			
			auto& item = e.getComponent<Item>();

			if (!item.catagory.test(TOOL_DIGGING) || e.hasComponent<Claimed>())
				return; // Should these be continue's?

			auto& co = e.getComponent<PositionComponent>().co;

			targets.emplace_back(getIdx(co));	
		});

		pick_map.update(targets);

		update_pick_map = false;
	}

	if (update_axe_map)
	{

		update_axe_map = false;
	}

	if (update_block_map)
	{
		std::unordered_set<int> repeats;
		std::vector<int> targets;

		itemHelper.forEachItem(
			[&targets, &repeats](const auto& e)
			{
				if (e.hasComponent<Item>() && e.getComponent<Item>().tag == "block")
				{
					if (e.hasComponent<Claimed>() || ! e.hasComponent<PositionComponent>())
						return; // Should these be continue's?

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
