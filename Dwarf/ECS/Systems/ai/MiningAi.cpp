#include "MiningAi.h"
#include "WorkTemplate.h"
#include "JobBoard.h"
#include "../Designations.h"
#include "../Tile.h"
#include "../MiningSystem.h"

namespace JobsBoard
{
	void evaluate_mining(JobBoard & board, const Entity & e, Coordinates co, JobEvaluatorBase * jt)
	{
		if (designations->mining.empty())
			return;

		// Pick distance evaluate

		// Find total distance for job
		int distance = miningMap[getIdx(co)]; // + PickDistance

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
		auto& mov = e.getComponent<MovementComponent>();

		// If entity is already moving, 
		// don't change it's destination
		if (mov.progress)
			return;

		auto& co = e.getComponent<PositionComponent>().co;
		const auto idx = getIdx(co);

		if (miningMap[idx] == 0)
		{
			tag.step = MiningTag::DIG;
			return;
		}
			
		int currentDir = 0;
		uint8_t min_value = std::numeric_limits<uint8_t>::max();

		if (miningMap[getIdx({co.x, co.y - 1, co.z})] < min_value && tileManager->CAN_GO_EAST)


			
		break;

	case MiningTag::DIG:

		break;

	case MiningTag::DROP_TOOL:
		work.cancel_work(e);
		break;
	}
}


