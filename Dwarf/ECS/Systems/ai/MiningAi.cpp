#include "MiningAi.h"
#include "WorkTemplate.h"
#include "JobBoard.h"
#include "../Designations.h"
#include "../Tile.h"

namespace JobsBoard
{
	void evaluate_mining(JobBoard & board, const Entity & e, Coordinates co, JobEvaluatorBase * jt)
	{
		if (designations->mining.empty())
			return;

		// Pick distance evaluate

		// Find total distance for job
		int distance = get_rough_distance(e.getComponent<PositionComponent>().co, co); // + pick distance;

		board.insert(std::make_pair(distance, jt));
	}
}

void MiningAi::init()
{
	JobsBoard::register_job_offer<MiningTag>(JobsBoard::evaluate_mining);
}

void MiningAi::update(double deltaT)
{
	auto& entities = getEntities();

	for (auto& e : entities)
	{
		updateMiner(e);
	}
}

void MiningAi::updateMiner(Entity e)
{
	WorkTemplate<MiningTag> work;

	auto& tag = e.getComponent<MiningTag>(); 

	switch (tag.step)
	{
	case MiningTag::GET_PICK:
		work.pickup_tool(e);

		// On success (which needs to be added in)
		tag.step = MiningTag::GOTO_SITE;
		break;

	case MiningTag::GOTO_SITE:
		if (work.followPath(e, tag))
			tag.step = MiningTag::DIG;
		break;

	case MiningTag::DIG:

		break;

	case MiningTag::DROP_TOOL:
		work.cancel_work(e);
		break;
	}
}


