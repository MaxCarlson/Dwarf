#pragma once
#include <map>
#include <functional>
#include "../Coordinates.h"
#include "../../World.h"
#include "ECS\Components\Sentients\AiWorkComponent.h"
#include <memory>



namespace JobsBoard
{
	struct JobEvaluatorBase;

	//using JobBoard = std::map<int, JobEvaluatorBase *>;

	// This is a struct instead of a pair so I can add
	// different rating structures easily in future
	// This is the last compirison done, job preferences are the
	// main force in job decisions and are used to index the JobBoard map
	struct JobRating
	{
		JobRating() = default;
		JobRating(int distance, JobEvaluatorBase *eval) : distance(distance), eval(eval) {}
		int distance;
		JobEvaluatorBase * eval;
	};

	using JobBoard = std::map<int, JobRating, std::greater<int>>;

	using JobEvaluator = std::function<void(JobBoard &, const Entity &, AiWorkComponent &, const Coordinates &, JobEvaluatorBase *)>;

	struct JobEvaluatorBase
	{
		virtual bool has_tag(Entity e) = 0;
		virtual void set_tag(Entity e) = 0;
		virtual void exec(JobBoard & board, const Entity& e, AiWorkComponent &prefs, const Coordinates & co) = 0;
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

		virtual void exec(JobBoard & board, const Entity& e, AiWorkComponent &prefs, const Coordinates & co) override final
		{
			eval_func(board, e, prefs, co, this);
		}
	};

	extern std::vector<std::unique_ptr<JobEvaluatorBase>> evaluators;


	template <typename T>
	inline void register_job_offer(JobEvaluator evaluator) {
		std::unique_ptr<JobEvaluatorBase> base = std::make_unique<JobEvaluatorConcrete<T>>(evaluator);

		evaluators.emplace_back(std::move(base));
	}

	inline void prefereceAndSubmitJob(JobBoard &board, AiWorkComponent &prefs, JobEvaluatorBase * jt, const std::string& jobSkill, const int distance)
	{
		// Find numerical job rating value for this type of work
		auto pfind = prefs.jobPrefrences.find(jobSkill);

		if (pfind->second < 1 || pfind == prefs.jobPrefrences.end())
			return;

		auto find = board.find(pfind->second);

		// Overwrite if distance to equally prefered job is less
		// or add if job preference doesn't exist
		if (find->second.distance > distance || find == board.end())
			board[pfind->second] = JobRating{ distance, jt };
	}

	bool is_working(Entity e);
	void evaluate(JobBoard &board, const Entity &entity, const Coordinates &co);
}

