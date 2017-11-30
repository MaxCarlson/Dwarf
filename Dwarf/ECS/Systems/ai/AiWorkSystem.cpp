#include "AiWorkSystem.h"
#include "JobBoard.h"
#include "../../Components/JobComponent.h"
#include "../../Components/PositionComponent.h"

#include "../../World.h"
#include "../../../Engine.h"

namespace JobsBoard
{
	JobBoard job_evaluations(const Entity &e, Coordinates &co) {
		JobBoard board;
		evaluate(board, e, co);

		return board;
	}
}

void AiWorkSystem::update()
{
	for (Entity e : getEntities())
	{
		if (JobsBoard::is_working(e))
			continue;

		auto& co = e.getComponent<PositionComponent>().co;

		// Find a list of availible jobs
		auto& availableJobs = JobsBoard::job_evaluations(e, co);

		// No Jobs!!
		if (availableJobs.empty())
			continue;


		auto jobType = availableJobs.begin()->second;

		jobType->set_tag(e);
	}
}
