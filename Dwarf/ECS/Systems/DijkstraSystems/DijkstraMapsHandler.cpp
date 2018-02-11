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
#include "../../Messages/harvest_map_changed_message.h"
#include "../../Messages/seeds_map_changed_message.h"

DijkstraMap pick_map;
DijkstraMap block_map;
DijkstraMap architecture_map;
DijkstraMap axe_map;
DijkstraMap harvest_map;
DijkstraMap seeds_map;


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
	subscribe_mbox<axemap_changed_message>();
	subscribe_mbox<block_map_changed_message>();
	subscribe_mbox<designate_architecture_message>();
	subscribe_mbox<harvest_map_changed_message>();
	subscribe_mbox<seeds_map_changed_message>();
}

void DijkstraMapsHandler::update(const double duration)
{
	each_mbox<update_all_maps_message>([this]( const update_all_maps_message &msg)
	{
		update_pick_map = true;
		update_axe_map = true;
		update_block_map = true;
		update_architecture = true;
		update_harvest = true;
		update_seeds = true;
	});
	each_mbox<pick_map_changed_message>([this](const pick_map_changed_message &msg)				 { update_pick_map	   = true; });
	each_mbox<axemap_changed_message>([this](const axemap_changed_message &msg)					 { update_axe_map      = true; });
	each_mbox<block_map_changed_message>([this](const block_map_changed_message & msg)			 { update_block_map    = true; });
	each_mbox<designate_architecture_message>([this](const designate_architecture_message & msg) { update_architecture = true; });
	each_mbox<harvest_map_changed_message>([this](const harvest_map_changed_message &msg)		 { update_harvest	   = true; });
	each_mbox<seeds_map_changed_message>([this](const seeds_map_changed_message &msg)			 { update_seeds        = true; });

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
		std::vector<int> targets;

		itemHelper.forEachItem([&targets](auto& e) {

			auto& item = e.getComponent<Item>();

			if (!item.catagory.test(TOOL_CHOPPING) || e.hasComponent<Claimed>())
				return; // Should these be continue's?

			auto& co = e.getComponent<PositionComponent>().co;

			targets.emplace_back(getIdx(co));
		});

		axe_map.update(targets);

		update_axe_map = false;
	}

	if (update_block_map)
	{
		std::unordered_set<int> repeats;
		std::vector<int> targets;

		itemHelper.forEachItem( [&targets, &repeats](const auto& e)
			{
				if (e.hasComponent<Item>() && e.getComponent<Item>().tag == "block")
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

	if (update_harvest)
	{
		std::vector<int> targets;

		for (const auto& h : designations->harvest)
			targets.emplace_back(getIdx(h.second));

		harvest_map.update(targets);
		update_harvest = false;
	}

	if (update_seeds)
	{
		std::unordered_set<int> repeats;
		std::vector<int> targets;

		itemHelper.forEachItem([&targets, &repeats](const auto& e)
		{
			if (e.hasComponent<Item>() && e.getComponent<Item>().catagory.test(ITEM_SEED))
			{
				if (e.hasComponent<Claimed>() || !e.hasComponent<PositionComponent>())
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
		update_seeds = false;
	}
}
