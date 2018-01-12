#include "stdafx.h"
#include "WoodcuttingAi.h"
#include "JobBoard.h"
#include "WorkTemplate.h"
#include "../helpers/ItemHelper.h"
#include "../../../Designations.h"
#include "../../../Map/Tile.h"
#include "../../Components/Sentients/Stats.h"

namespace JobsBoard
{
	void evaluate_woodcutting(JobBoard & board, const Entity & e, Coordinates co, JobEvaluatorBase * j)
	{
		if (designations->woodcutting.empty())
			return;

		auto axeDist = axe_map.get(getIdx(co));

		if (axeDist > MAX_DIJKSTRA_DISTANCE - 1)
			return;
	}
}

void WoodcuttingAi::init()
{
	JobsBoard::register_job_offer<LumberjacTag>(JobsBoard::evaluate_woodcutting);
}

void WoodcuttingAi::update(const double duration)
{
	for (auto e : getEntities())
	{
		doWork(e, duration);
	}
}

void WoodcuttingAi::doWork(Entity & e, const double & duration)
{

}
