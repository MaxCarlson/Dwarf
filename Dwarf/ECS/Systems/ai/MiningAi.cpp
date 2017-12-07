#include "MiningAi.h"
#include "WorkTemplate.h"
#include "JobBoard.h"
#include "../Designations.h"
#include "../Map/Tile.h"
#include "../MiningSystem.h"
#include "../DijkstraSystems/DijkstraMapsHandler.h"

#include "../../Messages/recalculate_mining_message.h"
#include "../../Messages/perform_mining_message.h"
#include "../../Messages/pick_map_changed_message.h"
#include "../../Messages/drop_item_message.h"
#include "../../../Raws/Defs/ItemDefs.h"

#include <iostream>

#include <libtcod.hpp>

namespace JobsBoard
{
	void evaluate_mining(JobBoard & board, const Entity & e, Coordinates co, JobEvaluatorBase * jt)
	{
		if (designations->mining.empty())
			return;

		// Pick distance evaluate
		auto pickDistance = pick_map.get(getIdx(co));

		// No pick close enough
		if (pickDistance > MAX_DIJKSTRA_DISTANCE - 1)
			return;

		// Other evaluations based on Entity skill
		// so we're not only looking at distance as a measure of 
		// who should do which work?

		// Find total distance for job
		int distance = miningMap[getIdx(co)] + pickDistance;

		board.insert(std::make_pair(distance, jt));
	}
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

void MiningAi::updateMiner(const Entity& e)
{
	WorkTemplate<MiningTag> work;

	auto& tag = e.getComponent<MiningTag>(); 
	auto& co = e.getComponent<PositionComponent>().co;

	if (tag.step == MiningTag::GET_PICK)
	{
		work.followMap(pick_map, e, co, [&e, &work]()
		{
			// On Failure
			work.cancel_work(e);
			return;

		}, [&tag, &work, &e, &co]
		{
			work.pickup_tool<pick_map_changed_message>(e, co, TOOL_DIGGING, tag.currentPick, [&e, &work]()
			{
				// On Failure
				work.cancel_work(e);
				return;

			}, [&tag] 
			{
				// On Success
				// Entity has already picked up tool
				tag.step = MiningTag::GOTO_SITE;
				return;
			});
		});
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
			return;
		}

		// Set movement components destination
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

		// Get Entity position index
		// get out mining targets idx using our position idx
		// and find out which type of mining we'd like to do
		const auto idx = getIdx(co);

		if (miningMap[idx] > 0) // This shouldn't be neccasary, why is it?
		{
			tag.step = MiningTag::GOTO_SITE;
			return;
		}
			

		const int targetIdx = miningTargets[idx];
		const auto coxx = idxToCo(targetIdx); // <<<<< DELETE WHEN DONE TESTING
		const auto targetMiningType = designations->mining[targetIdx];

		if (targetMiningType > 0)
		{
			// Emit message to perform mining
			emit(perform_mining_message{ e, targetIdx, targetMiningType });	
			return;
		}

		tag.step = MiningTag::DROP_TOOL; 
		return;
	}

	else if (tag.step == MiningTag::DROP_TOOL)
	{
		emit(drop_item_message{ SLOT_TOOL, e.getId().index, tag.currentPick, co });
		emit(pick_map_changed_message{});
		work.cancel_work(e);
		return;
	}
}


