#include "MiningAi.h"
#include "WorkTemplate.h"
#include "JobBoard.h"
#include "../Designations.h"
#include "../Map/Tile.h"
#include "../MiningSystem.h"
#include "../DijkstraSystems/DijkstraMapsHandler.h"
#include "../../Components/Sentients/Stats.h"
#include "../../Messages/recalculate_mining_message.h"
#include "../../Messages/perform_mining_message.h"
#include "../../Messages/pick_map_changed_message.h"
#include "../../Messages/drop_item_message.h"
#include "../../../Raws/Defs/ItemDefs.h"
#include "../../../Raws/raws.h"
#include "../../../Raws/Materials.h"
#include "../../../Raws/Defs/MaterialDef.h"

#include <iostream>

#include <libtcod.hpp>

using namespace region;

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

		// Add Other evaluations based on Entity skill
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
	const auto ents = getEntities();
	for (auto& e : ents)
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
		if (mov.progress || !mov.path.empty())
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

		if (miningMap[getIdx(CO_NORTH)] < min_value && flag(co, CAN_GO_NORTH)) { // Find a better system than this!! ~~ And run this system at not fullspeed, no need
			min_value = miningMap[getIdx(CO_NORTH)];							 // This systems makes miners walk close together and to the same spots leading to clumping of miners, find a way to avoid this!
			currentDir = NORTH;
		}
		if (miningMap[getIdx(CO_SOUTH)] < min_value && flag(co, CAN_GO_SOUTH)) {
			min_value = miningMap[getIdx(CO_SOUTH)];
			currentDir = SOUTH;
		}
		if (miningMap[getIdx(CO_EAST)] < min_value && flag(co, CAN_GO_EAST)) {
			min_value = miningMap[getIdx(CO_EAST)];
			currentDir = EAST;
		}
		if (miningMap[getIdx(CO_WEST)] < min_value && flag(co, CAN_GO_WEST)) {
			min_value = miningMap[getIdx(CO_WEST)];
			currentDir = WEST;
		}
		if (miningMap[getIdx(CO_NORTH_W)] < min_value && flag(co, CAN_GO_NORTH_W)) {
			min_value = miningMap[getIdx(CO_NORTH_W)];
			currentDir = NORTH_W;
		}
		if (miningMap[getIdx(CO_NORTH_E)] < min_value && flag(co, CAN_GO_NORTH_E)) {
			min_value = miningMap[getIdx(CO_NORTH_E)];
			currentDir = NORTH_E;
		}
		if (miningMap[getIdx(CO_SOUTH_E)] < min_value && flag(co, CAN_GO_SOUTH_E)) {
			min_value = miningMap[getIdx(CO_SOUTH_E)];
			currentDir = SOUTH_E;
		}
		if (miningMap[getIdx(CO_SOUTH_W)] < min_value && flag(co, CAN_GO_SOUTH_W)) {
			min_value = miningMap[getIdx(CO_SOUTH_W)];
			currentDir = SOUTH_W;
		}
		if (miningMap[getIdx(CO_UP)] < min_value && flag(co, CAN_GO_UP)) { // co.z < MAP_DEPTH - 1 ?
			min_value = miningMap[getIdx(CO_UP)];
			currentDir = UP;
		}
		if (miningMap[getIdx(CO_DOWN)] < min_value && flag(co, CAN_GO_DOWN)) {
			min_value = miningMap[getIdx(CO_DOWN)];
			currentDir = DOWN;
		}

		if (currentDir == NO_DIRECTION)
		{
			std::cout << "Cannot find mining path!!!" << std::endl;
			tag.step = MiningTag::DROP_TOOL;
			return;
		}

		// Set movement components destination
		Coordinates dest = co;

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

		mov.path.emplace_back(dest);
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
		const auto targetMiningType = designations->mining[targetIdx];

		if (targetMiningType > 0)
		{
			static const std::string jobSkill = "mining";

			auto& stats = e.getComponent<Stats>();

			// Target tile has health left
			if (tileHealth(targetIdx) > 0)
			{
				const auto dmg = static_cast<uint16_t>(doWorkDamage(stats, jobSkill));
				
				damageTile(targetIdx, dmg);
				return;
			} 

			// Target tile has no health left and is still designated,
			// let's destroy it and award points to miner
			//
			// We might need to add skill points to miners while they participate if 
			// only giving the points to the one who damages it last getting all the points
			// will cause imbalences
			else
			{
				giveWorkXp(stats, jobSkill, DIFFICULTY_MODERATE);

				// Remove the designation and change the tile
				designations->mining.erase(targetIdx);

				// Change tile to a floor, Probably don't want to do this
				// if tile is over open space?
				region::makeFloor(targetIdx);

				// Don't really want to spawn a stone boulder for each thing being mined do we?
				// might have to change
				auto matIdx = getTileMaterial(idxToCo(targetIdx));
				auto mat = getMaterial(matIdx);

				// Spawn items in amount denoted by material type
				for (int i = 0; i < mat->minesToAmount; ++i)
					spawnItemOnGround(mat->minesToTag, matIdx, idxToCo(targetIdx));

				//tileRecalcAll();
				spot_recalc_paths(idxToCo(targetIdx));

				emit(recalculate_mining_message{});
				return;
			}
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


