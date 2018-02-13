#include "stdafx.h"
#include "FarmingSoilAi.h"
#include "JobBoard.h"
#include "WorkTemplate.h"
#include "Designations.h"
#include "ECS\Systems\helpers\ItemHelper.h"

static const std::string jobSkill = "farming";

namespace JobsBoard
{
	void evaluate_farm_soil(JobBoard & board, const Entity & e, AiWorkComponent &prefs, const Coordinates& co, JobEvaluatorBase * jt)
	{
		if (designations->farming.empty())
			return;

		// Find numerical job rating value for this type of work
		auto pfind = prefs.jobPrefrences.find(jobSkill);

		if (pfind == prefs.jobPrefrences.end() || pfind->second < 1)
			return;

		for (const auto& f : designations->farming)
		{
			if (f.second.step == FarmInfo::ADD_SOIL)
			{
				auto find = board.find(pfind->second);

				const auto distance = static_cast<int>(get_3D_distance(co, idxToCo(f.first)));
				// Overwrite if distance to equally prefered job is less
				// or add if job preference doesn't exist
				if (find == board.end() || find->second.distance > distance)
					board[pfind->second] = JobRating{ distance, jt };
			}
		}
	}
}

void FarmingSoilAi::init()
{
	JobsBoard::register_job_offer<FarmSoilTag>(JobsBoard::evaluate_farm_soil);
}

void FarmingSoilAi::update(const double duration)
{
	for (const auto& e : getEntities())
	{

	}
}
