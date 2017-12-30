#pragma once
#include <map>
#include <functional>
#include "../Coordinates.h"
#include "../../World.h"
#include <memory>

namespace JobsBoard
{
	struct JobEvaluatorBase;

	using JobBoard = std::map<int, JobEvaluatorBase *>;

	using JobEvaluator = std::function<void(JobBoard &, const Entity &, Coordinates &, JobEvaluatorBase *)>;

	struct JobEvaluatorBase
	{
		virtual bool has_tag(Entity e) = 0;
		virtual void set_tag(Entity e) = 0;
		virtual void exec(JobBoard & board, const Entity e, Coordinates & co) = 0;
	};

	template<typename TAG>
	struct JobEvaluatorConcrete : public JobEvaluatorBase
	{
		JobEvaluatorConcrete(JobEvaluator func) : eval_func(func) {}

		// Functions scores job of type TAG
		const JobEvaluator eval_func;

		virtual bool has_tag(Entity e) override final
		{
			return e.hasComponent<TAG>();
		}

		virtual void set_tag(Entity e) override final
		{
			e.addComponent<TAG>();
			e.activate();
		}

		virtual void exec(JobBoard & board, const Entity e, Coordinates & co) override final
		{
			eval_func(board, e, co, this);
		}
	};

	extern std::vector<std::unique_ptr<JobEvaluatorBase>> evaluators;


	template <typename T>
	inline void register_job_offer(JobEvaluator evaluator) {
		std::unique_ptr<JobEvaluatorBase> base = std::make_unique<JobEvaluatorConcrete<T>>(evaluator);

		evaluators.emplace_back(std::move(base));
	}

	bool is_working(Entity e);
	void evaluate(JobBoard &board, const Entity &entity, Coordinates &co);
}

