#include "../../../stdafx.h"
#include "HaulingSystem.h"
#include "WorkTemplate.h"
#include "JobBoard.h"
#include "StockpileSystem.h"
#include "../helpers/ItemHelper.h"
#include "../../Components/Claimed.h"
#include "../../Components/PositionComponent.h"
#include "../../Messages/drop_item_message.h"



namespace JobsBoard
{
	void evaluate_hauling(JobBoard & board, const Entity & e, Coordinates co, JobEvaluatorBase * jt)
	{
		// No items to be hauled :(
		if (storeableItems.empty())
			return;

		// first storable co;
		const auto co2 = idxToCo(storeableItems.back().destination);


		// These heuristic below might need to be revised as all entities will think they're going for one item, when really they're going to be going to different ones

		// Distance to item
		const auto idist = region::get_3D_distance(co, co2); 

		// Total distance
		const int distance = int( idist + region::get_3D_distance(co2, e.getWorld().getEntity(storeableItems.back().itemId).getComponent<PositionComponent>().co) );


		// Add some other prefrences aside from distance,
		// Possibly make less skilled workers prefer hauling?
		// Low priority?

		board.insert(std::make_pair(distance, jt));
	}
}

void HaulingSystem::init()
{
	JobsBoard::register_job_offer<HaulingTag>(JobsBoard::evaluate_hauling);
}

void HaulingSystem::update(const double duration)
{
	auto& world = getWorld();

	for (auto& e : getEntities())
	{
		WorkTemplate<HaulingTag> work;
		auto& co = e.getComponent<PositionComponent>().co;
		auto& tag = e.getComponent<HaulingTag>();
		auto& mov = e.getComponent<MovementComponent>();

		if (tag.step == HaulingTag::FIND_JOB)
		{
			auto st = storeableItems.back();

			tag.currentItem = st.itemId;
			tag.destination = st.destination;

			auto& item = world.getEntity(st.itemId);

			if (item.isValid() && !item.hasComponent<Claimed>() && item.hasComponent<PositionComponent>())
			{
				itemHelper.claim_item(item);

				mov.destination = item.getComponent<PositionComponent>().co;
				tag.step = HaulingTag::GOTO_PIKCUP;

				std::cout << "Hauling item " << item.getComponent<Item>().name << " - " << " to " << st.destination << "\n";
			}
			else
				work.cancel_work(e);

			storeableItems.pop_back();
			return;
		}

		else if (tag.step == HaulingTag::GOTO_PIKCUP)
		{
			// Don't interrupt movement
			if (mov.progress)
				return;

			if (mov.path.empty() && getIdx(co) != itemHelper.get_item_location(tag.currentItem))
			{
				mov.destination = idxToCo(itemHelper.get_item_location(tag.currentItem));
				return;
			}


			if (mov.cannotFindPath)
			{
				work.cancel_work(e);
				mov.cannotFindPath = false;
			}

			// We're on top of the item!
			if (getIdx(co) == itemHelper.get_item_location(tag.currentItem))
			{
				tag.step = HaulingTag::PICKUP_ITEM;
				return;
			}

			if (!mov.path.empty())
				return;

			auto& item = world.getEntity(tag.currentItem);
			item.removeComponent<Claimed>();
			item.activate();

			work.cancel_work(e); // We might need to unclaim components???
		}

		else if (tag.step == HaulingTag::PICKUP_ITEM)
		{
			std::cout << "Picking up item haul - " << world.getEntity(tag.currentItem).getComponent<Item>().name << "\n";

			emit(pickup_item_message{ InventorySlots::SLOT_CARRYING, e.getId().index, tag.currentItem, 0 });
			tag.step = HaulingTag::GOTO_STOCKPILE;
		}

		else if (tag.step == HaulingTag::GOTO_STOCKPILE)
		{
			// Don't interrupt movement
			if (mov.progress)
				return;

			if (mov.path.empty() && getIdx(co) != tag.destination) // Delete this? Results in infinite loop if cannot find path atm
			{
				mov.destination = idxToCo(tag.destination);
				return;
			}

			if (mov.cannotFindPath)
			{
				itemHelper.unclaim_item_by_id(tag.currentItem);
				work.cancel_work(e);
				mov.cannotFindPath = false;
			}

			// We're on top of the item!
			if (getIdx(co) == itemHelper.get_item_location(tag.currentItem))
			{
				tag.step = HaulingTag::ADD_TO_STOCKPILE;
				return;
			}

			if (!mov.path.empty())
				return;


			itemHelper.unclaim_item_by_id(tag.currentItem);
			work.cancel_work(e); 
		}

		else if (tag.step == HaulingTag::ADD_TO_STOCKPILE)
		{
			itemHelper.unclaim_item_by_id(tag.currentItem);
			tag.currentItem = 0;

			emit(drop_item_message{ SLOT_CARRYING, e.getId().index, tag.currentItem, co });

			work.cancel_work(e);
		}
	}
}
