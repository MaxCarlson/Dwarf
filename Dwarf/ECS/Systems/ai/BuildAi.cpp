#include "BuildAi.h"
#include "JobBoard.h"
#include "../Designations.h"
#include "../../Components/PositionComponent.h"
#include "../../../Map/Tile.h"
#include "../../Components/Tags/BuilderTag.h"
#include "../../Components/MovementComponent.h"
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

	else if (tag.step == BuilderTag::FIND_COMPONENT)
	{
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