#include "stdafx.h"
#include "FarmingAi.h"
#include "JobBoard.h"
#include "Map\Tile.h"
#include "WorkTemplate.h"
#include "Designations.h"
#include "Raws\ReadPlants.h"
#include "Raws\Defs\PlantDef.h"
#include "ECS\Components\Sentients\Stats.h"
#include "../DijkstraSystems/DijkstraMapsHandler.h"
#include "ECS\Messages\planting_map_changed_message.h"
#include "ECS\Systems\helpers\SeedsHelper.h"
#include "ECS\Systems\helpers\PathFinding.h"
#include "ECS\Systems\helpers\ItemHelper.h"
#include "Raws\Defs\ItemDefs.h"

static const std::string jobSkill = "farming";


namespace JobsBoard
{
	void evaluate_farming(JobBoard & board, const Entity & e, AiWorkComponent &prefs, const Coordinates& co, JobEvaluatorBase * jt)
	{
		if (designations->planting.empty())
			return;

		// Find numerical job rating value for this type of work
		auto pfind = prefs.jobPrefrences.find(jobSkill);

		if (pfind->second < 1 || pfind == prefs.jobPrefrences.end())
			return;

		auto find = board.find(pfind->second);

		for (const auto& f : designations->farming)
		{
			if (f.second.step == FarmInfo::PLANT)
			{
				const auto distance = static_cast<int>(get_3D_distance(co, idxToCo(f.first)));
				// Overwrite if distance to equally prefered job is less
				// or add if job preference doesn't exist
				if (find == board.end() || find->second.distance > distance)
					board[pfind->second] = JobRating{ distance, jt };
			}
		}
	}
}

void FarmingAi::init()
{
	JobsBoard::register_job_offer<PlantingTag>(JobsBoard::evaluate_farming);
}

void findHoe(const Entity& e, MovementComponent &mov, WorkTemplate<PlantingTag> &work, const Coordinates& co, PlantingTag &tag);
void pickupHoe(const Entity& e, MovementComponent &mov, WorkTemplate<PlantingTag> &work, const Coordinates& co, PlantingTag &tag);
void findSeeds(const Entity& e, MovementComponent &mov, WorkTemplate<PlantingTag> &work, const Coordinates& co, PlantingTag &tag);
void gotoSeeds(const Entity& e, MovementComponent &mov, WorkTemplate<PlantingTag> &work, const Coordinates &co, PlantingTag &tag);

void FarmingAi::update(const double duration)
{
	for (const auto& e : getEntities())
	{
		WorkTemplate<PlantingTag> work;

		auto& co  = e.getComponent<PositionComponent>().co;
		auto& tag = e.getComponent<PlantingTag>();
		auto& mov = e.getComponent<MovementComponent>();

		switch (tag.step)
		{
		case PlantingTag::FIND_HOE:
			findHoe(e, mov, work, co, tag);
			break;

		case PlantingTag::PICKUP_HOE:
			pickupHoe(e, mov, work, co, tag);
			break;

		case PlantingTag::FIND_SEEDS:
			findSeeds(e, mov, work, co, tag);
			break;

		case PlantingTag::GOTO_SEEDS:
			gotoSeeds(e, mov, work, co, tag);
			break;

		case PlantingTag::FIND_PLANTING:

			break;

		case PlantingTag::PLANT:

			break;
		}
	}
}


void findHoe(const Entity & e, MovementComponent & mov, WorkTemplate<PlantingTag>& work, const Coordinates & co, PlantingTag & tag)
{
	work.findTool(e, co, mov, TOOL_FARMING, [&work, &tag, &e]()
	{
		// On failure
		work.cancel_work(e);

	}, [&tag]
	{
		// On holding item already
		tag.step = PlantingTag::FIND_SEEDS;
	
	}, [&tag, &co](size_t id)
	{
		// On needing to go grab item

		tag.itemId = id;
		tag.targetCo = world.getEntity(id).getComponent<PositionComponent>().co;
		tag.step = PlantingTag::PICKUP_HOE;
	});
}

void pickupHoe(const Entity & e, MovementComponent & mov, WorkTemplate<PlantingTag>& work, const Coordinates & co, PlantingTag & tag)
{
	work.followPath(mov, co, tag.targetCo, [&tag]()
	{
		// On path failure

	}, []
	{

	});
}

void findSeeds(const Entity & e, MovementComponent &mov, WorkTemplate<PlantingTag> &work, const Coordinates & co, PlantingTag & tag)
{
	std::map<int, std::pair<FarmInfo, Coordinates>> farms;
	for (const auto& f : designations->farming)
	{
		if (f.second.step == FarmInfo::PLANT && f.second.progress == 0.0)
		{
			auto dist = get_3D_distance(co, idxToCo(f.first));

			farms[dist] = std::make_pair(f.second, idxToCo(f.first));
		}
	}

	// Find seeds 
	for (const auto& s : farms)
	{
		const auto sId = s.second.first.seedId;

		// Farm designation already has a seed claimed
		if (sId > 0)
		{
			auto seed = world.getEntity(sId);
			const auto& seedCo = seed.getComponent<PositionComponent>().co;

			auto path = findPath(co, seedCo);

			if (!path->failed)
			{
				mov.path = path->path;
				tag.targetCo = seedCo;
				tag.step = PlantingTag::GOTO_SEEDS;
				return;
			}
		}

		// Farming designation has no seed claimed
		// find one and claim it
		else
		{
			seedsHelper.forAllSeeds([&co, &tag, &mov, &s](Entity e)
			{
				if (e.hasComponent<Claimed>() || !e.hasComponent<PositionComponent>())
					return;

				const auto& seedCo = e.getComponent<PositionComponent>().co;

				if (e.getComponent<Seed>().plantTag != s.second.first.seedType)
					return;

				auto path = findPath(co, seedCo);

				if (!path->failed)
				{
					mov.path = path->path;
					tag.targetCo = seedCo;
					tag.step = PlantingTag::GOTO_SEEDS;
					itemHelper.claim_item(e);
					return;
				}
			});
		}
	}

	// Failed finding seed or path
	work.cancel_work(e);
}

void gotoSeeds(const Entity & e, MovementComponent & mov, WorkTemplate<PlantingTag> &work, const Coordinates &co, PlantingTag & tag)
{
	work.followPath(mov, co, tag.targetCo, [&tag]()
	{
		// On pathing failure
		tag.step = PlantingTag::FIND_SEEDS;
		return;
	}, [&tag, &e]
	{
		// Found seed!
		tag.step = PlantingTag::
		world.emit(pickup_item_message{ SLOT_CARRYING, e.getId().index, tag.itemId, 0 });
		return;
	});
}
