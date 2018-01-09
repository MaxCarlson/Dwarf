#include "WorkOrders.h"
#include "JobBoard.h"
#include "WorkTemplate.h"
#include "../Designations.h"
#include "../ECS/Components/PositionComponent.h"
#include "../ECS/Components/Tags/WorkOrderTag.h"
#include "../ECS/Components/Sentients/Stats.h"
#include "../ECS/Systems/helpers/WorkOrderHelper.h"
#include "../ECS/Systems/helpers/ItemHelper.h"
#include "../Raws/ReadReactions.h"
#include "../Raws/raws.h"
#include "../ECS/Systems/helpers/PathFinding.h"
#include "../ECS/Messages/drop_item_message.h"
#include "../ECS/Messages/block_map_changed_message.h"

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
	const auto ents = getEntities();

	for (const auto& e : ents)
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

				// Set entity path
				auto path = findPath(co, idxToCo(cpos));
				
				// Can't find this component? Find another
				if (path->failed)
				{
					tag.current_component = 0;
					continue;		
				}

				mov.path = path->path;
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


		// We're on top of the item!
		if (getIdx(co) == itemHelper.get_item_location(tag.current_component))
		{
			tag.step = WorkOrderTag::GRAB_COMPONENT;
			return;
		}

		// Look for a path to the component
		// if there isn't one cancel working and return
		if (mov.path.empty())
		{
			auto path = findPath(co, idxToCo(itemHelper.get_item_location(tag.current_component)));

			if (path->failed)
			{
				work.cancel_work(e);
				return;
			}

			mov.path = path->path;
			return;
		}
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

		// Tell building it has the component
		// and procede to drop it off
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
			auto path = findPath(co, *pos);

			if (path->failed)
			{
				emit(drop_item_message{ SLOT_CARRYING, e.getId().index, tag.current_component, co });
				work.cancel_work(e);
				return;
			}

			mov.path = path->path;
			return;
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
		auto* reaction = getReaction(tag.reaction.reactionTag);

		auto& stats = e.getComponent<Stats>();

		//stats.skills.find(reaction->skill);

		if (tag.progress < 100.0) // Add in skill levels affecting time taken!!
		{
			doWorkTime(duration, tag.progress, reaction->difficulty);
			return;
		}

		auto skillCheck = skillRoll(stats, reaction->skill, reaction->difficulty);
	
		if (skillCheck >= SUCCESS)
		{
			auto& world = getWorld();
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

				itemHelper.deleteItem(comp.first);
			}

			// Produce outputs ~~ figure out how to deal with mixed outputs
			// in terms of affects
			for (auto & out : reaction->outputs)
				for (int i = 0; i < out.second; ++i)
				{
					std::cout << "Reaction spawning" << out.first << material << "\n";
					spawnItemOnGround(out.first, material, co);
				}

			emit(block_map_changed_message{});

			// Finish up
			workOrderHelper->unclaim_workshop(tag.reaction.workshop_id);
			work.cancel_work(e);
			return;
		}
		// Reset some progress for a skill roll failure
		else if (skillCheck == FAIL)
		{
			tag.progress = 50.0;
			return;
		}
		// Reset all progress for a CRITICAL_FAILURE
		else
		{
			tag.progress = 0.0;
			return;
		}
	}
}
