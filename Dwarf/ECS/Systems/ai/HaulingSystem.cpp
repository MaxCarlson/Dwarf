#include "../../../stdafx.h"
#include "HaulingSystem.h"
#include "WorkTemplate.h"
#include "JobBoard.h"
#include "StockpileSystem.h"
#include "../helpers/ItemHelper.h"
#include "../../Components/Claimed.h"
#include "../../Components/PositionComponent.h"
#include "../../Messages/drop_item_message.h"
#include "../../../Designations.h"
#include "../ECS/Systems/helpers/PathFinding.h"


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
		const auto idist = get_3D_distance(co, co2); 

		// Total distance
		const int distance = int( idist + get_3D_distance(co2, e.getWorld().getEntity(storeableItems.back().itemId).getComponent<PositionComponent>().co) );


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
	const auto ents = getEntities();
	for (auto& e : ents)
		doWork(e);	
}

void HaulingSystem::doWork(Entity e)
{
	static auto& world = getWorld();

//	if (e.getId().index == 7)
//		int a = 5;

	WorkTemplate<HaulingTag> work;
	auto& co = e.getComponent<PositionComponent>().co;
	const int idx = getIdx(co);

	auto& tag = e.getComponent<HaulingTag>();
	auto& mov = e.getComponent<MovementComponent>();

	if (tag.step == HaulingTag::FIND_JOB)
	{
		if (storeableItems.empty())
		{
			work.cancel_work(e);
			return;
		}

		auto st = storeableItems.back();

		tag.currentItem = st.itemId;
		tag.destination = st.destination;

		auto& item = world.getEntity(st.itemId);

		if (item.isValid() && !item.hasComponent<Claimed>() && item.hasComponent<PositionComponent>())
		{
			auto path = findPath(co, item.getComponent<PositionComponent>().co);

			if (path->failed)
			{
				work.cancel_work(e);
				return;
			}

			itemHelper.claim_item(item);

			mov.path = path->path;
			
			// Mark the stockpile square as taken
			// So we don't get multiple items set to be put on it
			designations->hauling[tag.destination] = region::stockpileId(tag.destination);

			std::cout << "Hauling item " << item.getComponent<Item>().name << " - " << " to " << st.destination << "\n";

			tag.step = HaulingTag::GOTO_PIKCUP;
			storeableItems.pop_back();
			return;
		}
		else
		{
			work.cancel_work(e);
			return;
		}
	}

	else if (tag.step == HaulingTag::GOTO_PIKCUP)
	{
		// Don't interrupt movement
		if (mov.progress)
			return;

		// We're on top of the item!
		if (idx == itemHelper.get_item_location(tag.currentItem))
		{
			tag.step = HaulingTag::PICKUP_ITEM;
			return;
		}

		if (mov.path.empty())
		{
			auto path = findPath(co, idxToCo(itemHelper.get_item_location(tag.currentItem)));

			if (path->failed)
			{

				itemHelper.unclaim_item_by_id(tag.currentItem);
				work.cancel_work(e); // We might need to unclaim components???
				return;
			}
		}
	}

	else if (tag.step == HaulingTag::PICKUP_ITEM)
	{
		std::cout << "Picking up item haul - " << world.getEntity(tag.currentItem).getComponent<Item>().name << " - " << tag.currentItem << "\n";

		emit(pickup_item_message{ InventorySlots::SLOT_CARRYING, e.getId().index, tag.currentItem, 0 });
		tag.step = HaulingTag::GOTO_STOCKPILE;
	}

	else if (tag.step == HaulingTag::GOTO_STOCKPILE)
	{
		// Don't interrupt movement
		if (mov.progress)
			return;

		// We're on top of the item!
		if (idx == tag.destination)
		{
			tag.step = HaulingTag::ADD_TO_STOCKPILE;
			return;
		}

		if (mov.path.empty())
		{
			auto path = findPath(co, idxToCo(tag.destination));

			if (path->failed)
			{

				emit(drop_item_message{ SLOT_CARRYING, e.getId().index, tag.currentItem, co });
				work.cancel_work(e);
				return;
			}

			mov.path = path->path;
			return;
		}
	}

	else if (tag.step == HaulingTag::ADD_TO_STOCKPILE)
	{
		emit(drop_item_message{ SLOT_CARRYING, e.getId().index, tag.currentItem, co });
		designations->hauling.erase(idx);

		work.cancel_work(e);
		return;
	}
}

