#include "WorkOrders.h"
#include "JobBoard.h"
#include "WorkTemplate.h"
#include "../Designations.h"
#include "../ECS/Components/PositionComponent.h"
#include "../ECS/Components/Tags/WorkOrderTag.h"
#include "../ECS/Systems/helpers/WorkOrderHelper.h"
#include "../ECS/Systems/helpers/ItemHelper.h"
#include "../Raws/ReadReactions.h"
#include "../Raws/raws.h"
#include "../ECS/Systems/helpers/PathFinding.h"
#include "../ECS/Messages/drop_item_message.h"

namespace JobsBoard
{
	void evaluate_work_order(JobBoard & board, const Entity & e, Coordinates co, JobEvaluatorBase * jt)
	{
		if (!designations->workOrders.empty())
		{
			board.insert(std::make_pair(15, jt));
			return;
		}
		
	}

}


void WorkOrders::init()
{
	JobsBoard::register_job_offer<WorkOrderTag>(JobsBoard::evaluate_work_order);
}

void WorkOrders::update(double duration)
{
	for (auto& e : getEntities())
		work(e, duration);
}

void WorkOrders::work(Entity e, const double& duration)
{
	WorkTemplate<WorkOrderTag> work;
	
	auto& tag = e.getComponent<WorkOrderTag>();
	auto& co = e.getComponent<PositionComponent>().co;
	auto& mov = e.getComponent<MovementComponent>();

	if (tag.step == WorkOrderTag::FIND_WORKSHOP)
	{
		std::unique_ptr<work_order_reaction> job;

		if (!designations->workOrders.empty())
		{
			job = workOrderHelper->find_work_order_reaction(tag);
		}

		if (!job)
		{
			work.cancel_work(e);
			return;
		}


		tag.reaction = *job;
		tag.step = WorkOrderTag::FIND_COMPONENT;
		return;
	}

	else if (tag.step == WorkOrderTag::FIND_COMPONENT)
	{
		std::vector<std::pair<std::size_t, bool>> components;

		bool hasComponents = true;
		for (auto & comp : tag.reaction.components)
		{
			// If the workshop doesn't have all components
			if (!comp.second)
			{
				hasComponents = false;
				tag.current_component = comp.first;

				auto cpos = itemHelper.get_item_location(comp.first);

				if (!cpos)
				{
					work.cancel_work(e);
					return;
				}

				// Set entity destination
				//mov.destination = idxToCo(cpos);
				auto path = findPath(co, idxToCo(cpos));
				
				if (!path->failed)
				{
					mov.path = path->path;
				}
				else
					continue;

				tag.step = WorkOrderTag::GOTO_COMPONENT;
				return;
			}
		}

		if (hasComponents)
		{
			tag.step = WorkOrderTag::WORK_WORKSHOP;
			return;
		}

		work.cancel_work(e);
		return;
	}

	else if (tag.step == WorkOrderTag::GOTO_COMPONENT)
	{
		// Don't interrupt movement
		if (mov.progress || !mov.path.empty())
			return;

		if (getIdx(co) != itemHelper.get_item_location(tag.current_component))
		{
			mov.destination = idxToCo(itemHelper.get_item_location(tag.current_component));
			return;
		}

		// We're on top of the item!
		if (getIdx(co) == itemHelper.get_item_location(tag.current_component))
		{
			tag.step = WorkOrderTag::GRAB_COMPONENT;
			return;
		}

		// Path didn't work out, try again. Possibly revisit this if there are issues
		// Testing canceling work instead
		//tag.step = WorkOrderTag::FIND_COMPONENT;

		work.cancel_work(e); // We might need to unclaim components???
		return;
	}

	else if (tag.step == WorkOrderTag::GRAB_COMPONENT)
	{
		emit(pickup_item_message{ InventorySlots::SLOT_CARRYING, e.getId().index, tag.current_component, 0 });
		tag.step = WorkOrderTag::GOTO_WORKSHOP;
	}

	else if (tag.step == WorkOrderTag::GOTO_WORKSHOP)
	{
		if (mov.progress || !mov.path.empty())
			return;

		auto building = getWorld().getEntity(tag.reaction.workshop_id);

		auto* pos = &building.getComponent<PositionComponent>().co;

		if (!pos)
		{
			emit(drop_item_message{ SLOT_CARRYING, e.getId().index, tag.current_component, co });
			work.cancel_work(e);
			return;
		}

		const auto dist = region::get_2D_distance(co, *pos);
		const bool zeq = co.z == pos->z;

		// Drop component and tell building it has the component
		if (co == *pos || (zeq && dist < 1.41))
		{
			for (auto & component : tag.reaction.components)
				if (tag.current_component == component.first)
					component.second = true;

			tag.step = WorkOrderTag::DROP_COMPONENT;
			return;
		}

		if (mov.path.empty())
		{
			emit(drop_item_message{ SLOT_CARRYING, e.getId().index, tag.current_component, co });
			work.cancel_work(e);
		}
	}
	else if (tag.step == WorkOrderTag::DROP_COMPONENT)
	{
		emit(drop_item_message{ SLOT_CARRYING, e.getId().index, tag.current_component, co });

		tag.current_component = 0;
		tag.step = WorkOrderTag::FIND_COMPONENT;
		return;
	}

	else if (tag.step == WorkOrderTag::WORK_WORKSHOP)
	{
		// Add in skills!!! // Add in time component

		auto& world = getWorld();

		auto reaction = getReaction(tag.reaction.reactionTag);
		
		// Delete component entities and capture data about
		// input items
		std::string materialNames = "";
		std::size_t material = 0;

		for (auto& comp : tag.reaction.components)
		{
			auto& cent = world.getEntity(comp.first);

			if (!cent.isValid() || !cent.hasComponent<Item>())
			{
				work.cancel_work(e);
				return;
			}
				
			material = cent.getComponent<Item>().material;
			materialNames += cent.getComponent<Item>().name + " ";
			//world.killEntity(cent);
			itemHelper.deleteItem(comp.first);
		}

		// Produce outputs ~~ figure out how to deal with mixed outputs
		// in terms of affects
		for(auto & out : reaction->outputs)
			for (int i = 0; i < out.second; ++i)
			{
				std::cout << "Reaction spawning" << out.first << material << "\n";
				spawnItemOnGround(out.first, material, co);
			}

		// Finish up
		workOrderHelper->unclaim_workshop(tag.reaction.workshop_id);
		work.cancel_work(e);
	}
}
