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
	void evaluate_woodcutting(JobBoard & board, const Entity & e, Coordinates co, JobEvaluatorBase * jt)
	{
		if (designations->woodcutting.empty())
			return;

		auto axeDist = axe_map.get(getIdx(co));

		if (axeDist > MAX_DIJKSTRA_DISTANCE - 1)
			return;

		int i = 0;

		auto treeDist = std::numeric_limits<double>().max();

		// Find the closest tree to Entity
		for (const auto& des : designations->woodcutting)
		{
			const auto d = region::get_3D_distance(co, des.second);

			if (d < treeDist)
				treeDist = d;		
		}

		board.insert(std::make_pair(axeDist + treeDist, jt));
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
