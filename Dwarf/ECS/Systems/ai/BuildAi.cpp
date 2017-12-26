#include "BuildAi.h"
#include "JobBoard.h"
#include "../Designations.h"
#include "../../Components/PositionComponent.h"
#include "../../../Map/Tile.h"
#include "../../Components/Tags/BuilderTag.h"
#include "../../Components/MovementComponent.h"
#include "../helpers/ItemHelper.h"
#include "WorkTemplate.h"
#include "../ECS/Messages/pickup_item_message.h"
#include "../ECS/Components/Sentients/Inventory.h"
#include <utility>


namespace JobsBoard
{
	void evaluate_building(JobBoard & board, const Entity & e, Coordinates co, JobEvaluatorBase * jt)
	{
		if (designations->buildings.empty())
			return;

		auto building_d = designations->buildings.back();
		auto id = building_d.entity_id; 

		auto building = e.getWorld().getEntity(id);

		auto* pos = &building.getComponent<PositionComponent>().co;

		// Add more factors than just distance for determaning prefrence
		
		if (pos)
		{
			auto distance = region::get_3D_distance(co, *pos);

			board.insert(std::make_pair(distance, jt));
		}
	}
}

void BuildAi::init()
{
	JobsBoard::register_job_offer<BuilderTag>(JobsBoard::evaluate_building);
}

void BuildAi::update(double duration)
{
	for (auto e : getEntities())
	{
		doBuild(e);
	}
}

void BuildAi::doBuild(const Entity & e)
{
	WorkTemplate<BuilderTag> work;

	auto& tag = e.getComponent<BuilderTag>();
	auto& co = e.getComponent<PositionComponent>().co;
	auto mov = e.getComponent<MovementComponent>();

	if (tag.step == BuilderTag::FIND_BUILDING)
	{
		if (designations->buildings.empty())
		{
			work.cancel_work(e);
			return;
		}

		tag.buildingTarget = designations->buildings.back();
		designations->buildings.pop_back(); 
		
		tag.step = BuilderTag::FIND_COMPONENT;
	}

	else if (tag.step == BuilderTag::FIND_COMPONENT)
	{
		bool hasComps = true;
		for (auto & component : tag.buildingTarget.componentIds)
		{
			if (!component.second)
			{
				hasComps = false;
				tag.current_component = component.first;

				auto pos = itemHelper.get_item_location(component.second);
				if (!pos)
				{
					designations->buildings.push_back(tag.buildingTarget);
					work.cancel_work(e);
					return;
				}

				// Set destination, pathing will be done later
				mov.destination = idxToCo(pos);

				tag.step = BuilderTag::GOTO_COMPONENT;
				// Possibly test for path here to avoid convuluted mov system?
				
			}
		}

		if (hasComps)
			tag.step = BuilderTag::BUILD_BUILDING;

		return;
	}

	else if (tag.step == BuilderTag::GOTO_COMPONENT)
	{
		// Don't interrupt movement
		if (mov.progress)
			return;

		if (mov.path.empty() && getIdx(co) != itemHelper.get_item_location(tag.current_component))
		{
			mov.destination = idxToCo(itemHelper.get_item_location(tag.current_component));
			return;
		}
			

		if (mov.cannotFindPath)
		{
			work.cancel_work(e);
			mov.cannotFindPath = false;
			designations->buildings.push_back(tag.buildingTarget);
		}

		// We're on top of the item!
		if (mov.path.empty() && getIdx(co) == itemHelper.get_item_location(tag.current_component))
		{
			tag.step = BuilderTag::GRAB_COMPONENT;
			return;
		}

		// Path didn't work out, try again. Possibly revisit this if there are issues
		tag.step = BuilderTag::FIND_COMPONENT;
		return;
	}

	else if (tag.step == BuilderTag::GRAB_COMPONENT)
	{
		emit(pickup_item_message{ InventorySlots::SLOT_CARRYING, e.getId().index, tag.current_component, 0 });
		tag.step = BuilderTag::GOTO_BUILDING;
	}

	else if (tag.step == BuilderTag::GOTO_BUILDING)
	{
		if (mov.progress || !mov.path.empty())
			return;

		auto building = getWorld().getEntity(tag.buildingTarget.entity_id);

		auto* pos = &building.getComponent<PositionComponent>().co;

		if (!pos)
		{
			work.cancel_work(e);
			return;
		}

		if (co != *pos)
		{
			mov.destination = *pos;
			return;
		}
		
		const auto dist = region::get_2D_distance(co, *pos);
		const bool zeq = co.z == pos->z;

		if (co == *pos || (zeq && dist < 1.41))
			tag.step = BuilderTag::BUILD_BUILDING;
	}
}
