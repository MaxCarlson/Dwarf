#include "MiningAi.h"
#include "WorkTemplate.h"
#include "JobBoard.h"
#include "../Designations.h"
#include "../Tile.h"
#include "../MiningSystem.h"

#include "../../Messages/recalculate_mining_message.h"

#include <iostream>

namespace JobsBoard
{
	void evaluate_mining(JobBoard & board, const Entity & e, Coordinates co, JobEvaluatorBase * jt)
	{
		if (designations->mining.empty())
			return;

		// Pick distance evaluate

		// Other evaluations based on Entity skill
		// so we're not only looking at distance as a measure of 
		// who should do which work?

		// Find total distance for job
		int distance = miningMap[getIdx(co)]; // + PickDistance

		board.insert(std::make_pair(distance, jt));
	}
}

MiningAi::MiningAi(TileManager * tileManager) : tileManager(tileManager)
{
}

void MiningAi::init()
{
	JobsBoard::register_job_offer<MiningTag>(JobsBoard::evaluate_mining);
}

void MiningAi::update(double deltaT)
{
	for (auto& e : getEntities())
	{
		updateMiner(e);
	}
}

void MiningAi::updateMiner(Entity e)
{
	WorkTemplate<MiningTag> work;

	auto& tag = e.getComponent<MiningTag>(); 
	auto& co = e.getComponent<PositionComponent>().co;

	if (tag.step == MiningTag::GET_PICK)
	{
		// Not implemented yet as we have no tools!!!!!!@!@
		work.pickup_tool(e);

		// On success (which needs to be added in)
		tag.step = MiningTag::GOTO_SITE;
	}

	else if (tag.step == MiningTag::GOTO_SITE)
	{
		auto& mov = e.getComponent<MovementComponent>();

		// If entity is already moving, 
		// don't change it's destination
		if (mov.progress)
			return;

		// If the mining map at our idx is zero,
		// we're at a mining site!
		const auto idx = getIdx(co);
		if (miningMap[idx] == 0)
		{
			tag.step = MiningTag::DIG;
			return;
		}

		int currentDir = NO_DIRECTION;
		uint8_t min_value = std::numeric_limits<uint8_t>::max();

		if (miningMap[getIdx(positionAt<NORTH>(co))] < min_value && tileManager->canGo<CAN_GO_NORTH>(co)) { // Find a better system than this!!
			min_value = miningMap[getIdx(positionAt<NORTH>(co))];
			currentDir = NORTH;
		}
		if (miningMap[getIdx(positionAt<SOUTH>(co))] < min_value && tileManager->canGo<CAN_GO_SOUTH>(co)) {
			min_value = miningMap[getIdx(positionAt<SOUTH>(co))];
			currentDir = SOUTH;
		}
		if (miningMap[getIdx(positionAt<EAST>(co))] < min_value && tileManager->canGo<CAN_GO_EAST>(co)) {
			min_value = miningMap[getIdx(positionAt<EAST>(co))];
			currentDir = EAST;
		}
		if (miningMap[getIdx(positionAt<WEST>(co))] < min_value && tileManager->canGo<CAN_GO_WEST>(co)) {
			min_value = miningMap[getIdx(positionAt<WEST>(co))];
			currentDir = WEST;
		}
		if (miningMap[getIdx(positionAt<NORTH_W>(co))] < min_value && tileManager->canGo<CAN_GO_NORTH_W>(co)) {
			min_value = miningMap[getIdx(positionAt<NORTH_W>(co))];
			currentDir = NORTH_W;
		}
		if (miningMap[getIdx(positionAt<NORTH_E>(co))] < min_value && tileManager->canGo<CAN_GO_NORTH_E>(co)) {
			min_value = miningMap[getIdx(positionAt<NORTH_E>(co))];
			currentDir = NORTH_E;
		}
		if (miningMap[getIdx(positionAt<SOUTH_E>(co))] < min_value && tileManager->canGo<CAN_GO_SOUTH_E>(co)) {
			min_value = miningMap[getIdx(positionAt<SOUTH_E>(co))];
			currentDir = SOUTH_E;
		}
		if (miningMap[getIdx(positionAt<SOUTH_W>(co))] < min_value && tileManager->canGo<CAN_GO_SOUTH_W>(co)) {
			min_value = miningMap[getIdx(positionAt<SOUTH_W>(co))];
			currentDir = SOUTH_W;
		}
		if (miningMap[getIdx(positionAt<UP>(co))] < min_value && tileManager->canGo<CAN_GO_UP>(co)) {
			min_value = miningMap[getIdx(positionAt<UP>(co))];
			currentDir = UP;
		}
		if (miningMap[getIdx(positionAt<DOWN>(co))] < min_value && tileManager->canGo<CAN_GO_DOWN>(co)) {
			min_value = miningMap[getIdx(positionAt<DOWN>(co))];
			currentDir = DOWN;
		}

		if (currentDir == NO_DIRECTION)
		{
			std::cout << "Cannot find mining path!!!" << std::endl;
			tag.step = MiningTag::DROP_TOOL;
		}

		mov.destination = co;
		Coordinates & dest = mov.destination;

		switch (currentDir)
		{
		case NORTH:   --dest.y; break;
		case SOUTH:   ++dest.y; break;
		case EAST:    ++dest.x; break;
		case WEST:    --dest.x; break;
		case NORTH_W: --dest.y; --dest.x; break;
		case NORTH_E: --dest.y; ++dest.x; break;
		case SOUTH_E: ++dest.y; ++dest.x; break;
		case SOUTH_W: ++dest.y; --dest.x; break;
		case UP:      ++dest.z; break;
		case DOWN:    --dest.z; break;
		}
		return;
	}

	else if (tag.step == MiningTag::DIG)
	{
		
		const auto idx = getIdx(co);

		const int targetIdx = miningTargets[idx];
		const int targetMiningType = designations->mining[targetIdx];

		if (targetMiningType > 0)
		{

			// Perform mining

			designations->mining.erase(targetIdx);

			// Recalculate mining map
			emit(recalculate_mining_message{});
		}

		tag.step = MiningTag::DROP_TOOL;
		return;
	}

	else if (tag.step == MiningTag::DROP_TOOL)
	{
		work.cancel_work(e);
		return;
	}
}


