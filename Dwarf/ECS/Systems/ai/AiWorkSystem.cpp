#include "AiWorkSystem.h"
#include "JobBoard.h"
#include "../../Components/JobComponent.h"
#include "../../Components/PositionComponent.h"

namespace JobsBoard
{
	JobBoard job_evaluations(const Entity &e, Coordinates &co) {
		JobBoard board;
		evaluate(board, e, co);

		return board;
	}
}


AiWorkSystem::AiWorkSystem()
{
}


AiWorkSystem::~AiWorkSystem()
{
}

void AiWorkSystem::update()
{
	auto& entities = getEntities();

	for (auto& e : entities)
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
