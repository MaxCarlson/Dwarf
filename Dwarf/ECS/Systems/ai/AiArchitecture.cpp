#include "AiArchitecture.h"
#include "JobBoard.h"
#include "../../Components/Tags/ArchitectTag.h"
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

}
