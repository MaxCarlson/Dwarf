#include "AiArchitecture.h"
#include "JobBoard.h"
#include "../../Messages/designate_architecture_message.h"

namespace JobsBoard
{
	void evaluate_architecture(JobBoard & board, const Entity & e, Coordinates co, JobEvaluatorBase * jt)
	{

	}
}

void AiArchitecture::init()
{
	JobsBoard::register_job_offer<ArchitectTag>(JobsBoard::evaluate_architecture);
}

void AiArchitecture::update(double duration)
{
	const auto& ents = getEntities();

	for (const auto& e : ents)
		doWork(e);
}

void AiArchitecture::doWork(Entity e)
{
}
