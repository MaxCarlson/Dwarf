#include "WorkOrders.h"
#include "JobBoard.h"
#include "WorkTemplate.h"
#include "Designations\WorkOrderDesignation.h"
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
#include "ECS\Components\Sentients\AiWorkComponent.h"
#include "ECS\Components\Quality.h"

namespace JobsBoard
{
	void evaluate_work_order(JobBoard & board, const Entity & e, AiWorkComponent &prefs, const Coordinates& co, JobEvaluatorBase * jt)
	{
		if (!workOrders.active.empty() && workOrderHelper->claimedWorkshops() < workOrders.active.size()) 
		{

			const auto [bestIdx, distance] = workOrderHelper->scanForBestWorkOrder(prefs);
			
			if (bestIdx < 1)
				return;

			auto find = board.find(bestIdx);

			// Overwrite if distance to equally prefered job is greater
			// or add if job preference doesn't exist
			if (find == board.end() || find->second.distance > distance)
				board[bestIdx] = JobRating{ distance, jt };
		}	
	}
}


void WorkOrders::init()
{
	JobsBoard::register_job_offer<WorkOrderTag>(JobsBoard::evaluate_work_order);
}

void findWorkshop(const Entity &e, const Coordinates &co, WorkOrderTag &tag, WorkTemplate<WorkOrderTag> &work, MovementComponent &mov);
void findComponent(const Entity &e, const Coordinates &co, WorkOrderTag &tag, WorkTemplate<WorkOrderTag> &work, MovementComponent &mov);
inline void gotoComponent(const Entity &e, const Coordinates &co, WorkOrderTag &tag, WorkTemplate<WorkOrderTag> &work, MovementComponent &mov);
inline void gotoWorkshop(const Entity &e, const Coordinates &co, WorkOrderTag &tag, WorkTemplate<WorkOrderTag> &work, MovementComponent &mov);
void workWorkshop(const Entity &e, const Coordinates &co, WorkOrderTag &tag, WorkTemplate<WorkOrderTag> &work, MovementComponent &mov, const double &duration);


void WorkOrders::update(const double duration)
{
	for (const auto& e : getEntities())
	{
		WorkTemplate<WorkOrderTag> work;
		auto& tag = e.getComponent<WorkOrderTag>();
		auto& mov = e.getComponent<MovementComponent>();
		const auto& co = e.getComponent<PositionComponent>().co;

		switch (tag.step)
		{
		case WorkOrderTag::FIND_WORKSHOP:
			findWorkshop(e, co, tag, work, mov);
			break;

		case WorkOrderTag::FIND_COMPONENT:
			findComponent(e, co, tag, work, mov);
			break;

		case WorkOrderTag::GOTO_COMPONENT:
			gotoComponent(e, co, tag, work, mov);
			break;

		case WorkOrderTag::GOTO_WORKSHOP:
			gotoWorkshop(e, co, tag, work, mov);
			break;

		case WorkOrderTag::WORK_WORKSHOP:
			workWorkshop(e, co, tag, work, mov, duration);
			break;
		}
	}
}


void findWorkshop(const Entity & e, const Coordinates & co, WorkOrderTag & tag, WorkTemplate<WorkOrderTag>& work, MovementComponent & mov)
{
	std::unique_ptr<work_order_reaction> job;

	if (!workOrders.active.empty())
	{
		job = workOrderHelper->findWorkOrderReaction(tag);
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

inline void unclaimComponentsAndWorkshop(WorkOrderTag &tag) // TODO:
{

}

void findComponent(const Entity & e, const Coordinates & co, WorkOrderTag & tag, WorkTemplate<WorkOrderTag>& work, MovementComponent & mov)
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
			tag.compCo = idxToCo(cpos);
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

inline void gotoComponent(const Entity & e, const Coordinates & co, WorkOrderTag & tag, WorkTemplate<WorkOrderTag>& work, MovementComponent & mov)
{
	work.followPath(mov, co, tag.compCo, [&e, &work]()
	{
		work.cancel_work(e); // TODO: Need to unclaim all components and workshop!
	}, [&]
	{
		auto path = findPath(co, tag.reaction.workshopCo);

		if (path->failed)
		{
			work.cancel_work(e); // TODO: Need to unclaim all components and workshop!
			return;
		}

		mov.path = path->path;
		world.emit(pickup_item_message { InventorySlots::SLOT_CARRYING, e.getId().index, tag.current_component, 0 });
		tag.step = WorkOrderTag::GOTO_WORKSHOP;
	});
}

inline void gotoWorkshop(const Entity & e, const Coordinates & co, WorkOrderTag & tag, WorkTemplate<WorkOrderTag>& work, MovementComponent & mov)
{
	work.followPath(mov, co, tag.reaction.workshopCo, [&work, &e]()
	{
		work.cancel_work(e); // TODO: Need to unclaim + drop all components and workshop!
	}, [&]
	{
		tag.step = WorkOrderTag::FIND_COMPONENT;
		world.emit(drop_item_message { SLOT_CARRYING, e.getId().index, tag.current_component, co, false });
	});
}

void workWorkshop(const Entity & e, const Coordinates & co, WorkOrderTag & tag, WorkTemplate<WorkOrderTag>& work, MovementComponent & mov, const double &duration)
{
	auto* reaction = getReaction(tag.reaction.reactionTag);

	auto& stats = e.getComponent<Stats>();

	if (tag.progress < reaction->time)
	{
		doWorkTime(stats, reaction->skill, duration, tag.progress);
		return;
	}

	giveWorkXp(stats, reaction->skill, reaction->difficulty);

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

	// If the output material was customized by player be sure to produce that
	// type of material. Material will only be customizable where it makes sense,
	// like with single output items.
	if (tag.reaction.outputMaterial != 0)
		material = tag.reaction.outputMaterial;

	// Produce outputs ~~ figure out how to deal with mixed outputs
	// in terms of affects
	for (auto & out : reaction->outputs) // TODO: Once stacking is enabled just create a stack of reaction items as one entity
		for (int i = 0; i < out.second; ++i)
		{
			auto quality = calculateQuality(stats, reaction->skill, reaction->difficulty);
			
			std::cout << "Reaction spawning" << out.first << material << "\n";
			spawnItemOnGround(out.first, material, co, SpawnColor::MATERIAL_COLOR, quality);
		}

	world.emit(block_map_changed_message {});

	// Finish up
	workOrderHelper->unclaim_workshop(tag.reaction.workshop_id);
	work.cancel_work(e);
	return;
}

/*
void WorkOrders::work(Entity e, const double& duration)
{
	WorkTemplate<WorkOrderTag> work;

	static auto& world = getWorld();

	auto& tag = e.getComponent<WorkOrderTag>();
	auto& co  = e.getComponent<PositionComponent>().co;
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

		const auto dist = get_2D_distance(co, *pos);
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

		if (tag.progress < reaction->time) 
		{
			doWorkTime(stats, reaction->skill, duration, tag.progress);
			return;
		}

		giveWorkXp(stats, reaction->skill, reaction->difficulty);

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
		for (auto & out : reaction->outputs) // TODO: Once stacking is enabled just create a stack of reaction items as one entity
			for (int i = 0; i < out.second; ++i)
			{
				auto quality = calculateQuality(stats, reaction->skill, reaction->difficulty);

				std::cout << "Reaction spawning" << out.first << material << "\n";
				spawnItemOnGround(out.first, material, co, SpawnColor::MATERIAL_COLOR, quality);
			}

		emit(block_map_changed_message{});

		// Finish up
		workOrderHelper->unclaim_workshop(tag.reaction.workshop_id);
		work.cancel_work(e);
		return;	
	}
}
*/
