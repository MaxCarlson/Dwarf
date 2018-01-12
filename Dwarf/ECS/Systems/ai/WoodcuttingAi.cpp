#include "stdafx.h"
#include "WoodcuttingAi.h"
#include "JobBoard.h"
#include "WorkTemplate.h"
#include "../helpers/ItemHelper.h"

namespace JobsBoard
{
	void evaluate_woodcutting(JobBoard & board, const Entity & e, Coordinates co, JobEvaluatorBase * j)
	{

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
