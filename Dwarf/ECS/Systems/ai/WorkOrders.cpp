#include "WorkOrders.h"
#include "JobBoard.h"
#include "../ECS/Components/Tags/WorkOrderTag.h"

namespace JobsBoard
{
	void evaluate_work_order(JobBoard & board, const Entity & e, Coordinates co, JobEvaluatorBase * jt)
	{

	}

}


void WorkOrders::init()
{
	JobsBoard::register_job_offer<WorkOrderTag>(JobsBoard::evaluate_work_order);
}

void WorkOrders::update(double duration)
{
	
}
