#include "JobBoard.h"

namespace JobsBoard
{
	std::vector<std::unique_ptr<JobEvaluatorBase>> evaluators;

	// If the Entity has any of the work tags which
	// have a function pointer tied to them
	// Entity is working
	bool is_working(const Entity & e)
	{
		for (const auto &job : evaluators) {
			if (job->has_tag(e)) 
				return true;
		}
		return false;
	}

	// This needs to be spruced up and take into account
	// Entity stats on function side!!!! Or possible here?
	void evaluate(JobBoard & board, const Entity & entity, Coordinates & co)
	{
		std::for_each(evaluators.begin(), evaluators.end(),
			[&board, &entity, &co](const auto &j) {
			j->exec(board, entity, co);
		});
	}
}


