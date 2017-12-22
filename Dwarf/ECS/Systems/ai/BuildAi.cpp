#include "BuildAi.h"
#include "JobBoard.h"
#include "../Designations.h"
#include "../../Components/PositionComponent.h"
#include "../../../Map/Tile.h"
#include "../../Components/Tags/BuilderTag.h"

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
}
