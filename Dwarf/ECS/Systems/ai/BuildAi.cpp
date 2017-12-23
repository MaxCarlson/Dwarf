#include "BuildAi.h"
#include "JobBoard.h"
#include "../Designations.h"
#include "../../Components/PositionComponent.h"
#include "../../../Map/Tile.h"
#include "../../Components/Tags/BuilderTag.h"
#include "../../Components/MovementComponent.h"
#include "../helpers/ItemHelper.h"
#include "WorkTemplate.h"


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
			auto distance = region::get_rough_distance(co, *pos);

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

	else if (tag.step == BuilderTag::SET_BUILDING_COMPONENTS) // Move this step into gui related to buildings once implemented?
	{
		bool hasComps = true;
		for (auto & component : tag.buildingTarget.componentIds)
			if (!component.second)
				hasComps = false;

		if (hasComps)
		{
			tag.step = BuilderTag::FIND_COMPONENT;
			return;
		}

	
		const auto buildingPos = tag.buildingTarget.co;		

		for (auto & component : tag.buildingTarget.components) // ~~ This is a placeholder, there is definitely a better, faster way to search for matching items
		{
			Entity item;

			int dist = std::numeric_limits<int>::max();

			// Find closest items to building that fit criteria
			// then set entity searching for component
			itemHelper.forEachItem([&dist, &item, &buildingPos, &component](auto type)
			{
				auto* itemPos = &type.getComponent<PositionComponent>().co;

				if (itemPos)
				{
					auto* it = &type.getComponent<Item>();
					
					// If the item matches the requirements of the building
					if (it && it.tag == component.tag 
						&& it.catagory == component.catagory
						&& it.material == component.req_material)
					{

						int tdist = region::get_rough_distance(buildingPos, *itemPos);
						if (tdist < dist)
						{
							dist = tdist;
							item = type;
						}
					}
				}
			});
			tag.buildingTarget.c
		}
	}

	else if (tag.step == BuilderTag::FIND_COMPONENT)
	{
		auto mov = e.getComponent<MovementComponent>();

		// Don't interrupt entity that's navigating it's path
		//if (!mov.path.empty())
			//return;

		bool hasComps = true;
		for (auto & component : tag.buildingTarget.componentIds)
		{
			if (!component.second)
			{
				hasComps = false;


			}
		}

		if (hasComps)
			tag.step = BuilderTag::GOTO_BUILDING;

		return;
	}
}
