#include "stdafx.h"
#include "FarmingSoilAi.h"
#include "JobBoard.h"
#include "WorkTemplate.h"
#include "Designations.h"
#include "Raws\Defs\ItemDefs.h"
#include "Raws\Materials.h"
#include "Raws\Defs\MaterialDef.h"
#include "ECS\Systems\helpers\PathFinding.h"
#include "ECS\Systems\helpers\ItemHelper.h"
#include "ECS\Components\Sentients\Inventory.h"
#include "ECS\Components\Sentients\Stats.h"
#include "ECS\Messages\drop_item_message.h"

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

void findSoil(const Entity& e, MovementComponent &mov, const Coordinates& co, WorkTemplate<FarmSoilTag> &work, FarmSoilTag &tag);
void gotoSoil(const Entity& e, MovementComponent &mov, const Coordinates& co, WorkTemplate<FarmSoilTag> &work, FarmSoilTag &tag);
void findFarm(const Entity& e, MovementComponent &mov, const Coordinates& co, WorkTemplate<FarmSoilTag> &work, FarmSoilTag &tag);
void gotoFarm(const Entity& e, MovementComponent &mov, const Coordinates& co, WorkTemplate<FarmSoilTag> &work, FarmSoilTag &tag);
void spreadSoil(const Entity& e, const double& duration, MovementComponent &mov, const Coordinates& co, WorkTemplate<FarmSoilTag> &work, FarmSoilTag &tag);

void FarmingSoilAi::update(const double duration)
{
	for (const auto& e : getEntities())
	{
		WorkTemplate<FarmSoilTag> work;
		auto& co = e.getComponent<PositionComponent>().co;
		auto& tag = e.getComponent<FarmSoilTag>();
		auto& mov = e.getComponent<MovementComponent>();


		switch (tag.step)
		{
		case FarmSoilTag::FIND_SOIL:
			findSoil(e, mov, co, work, tag);
			break;

		case FarmSoilTag::GOTO_SOIL:
			gotoSoil(e, mov, co, work, tag);
			break;

		case FarmSoilTag::FIND_FARM:
			findFarm(e, mov, co, work, tag);
			break;

		case FarmSoilTag::GOTO_FARM:
			gotoFarm(e, mov, co, work, tag);
			break;

		case FarmSoilTag::SPREAD_SOIL:
			spreadSoil(e, duration, mov, co, work, tag);
			break;
		}
	}
}

void findSoil(const Entity & e, MovementComponent &mov, const Coordinates & co, WorkTemplate<FarmSoilTag>& work, FarmSoilTag & tag)
{
	if (!itemHelper.isItemCatagoryAvailible<ITEM_SOIL>())
	{
		// Item type does not exist
		work.cancel_work(e);
		return;
	}

	// Let's claimed item
	tag.soilId = itemHelper.findClosestItemTypeClaimIt(e, ITEM_SOIL, co);
	tag.soilCo = world.getEntity(tag.soilId).getComponent<PositionComponent>().co;

	auto path = findPath(co, tag.soilCo);

	if (path->failed) 
	{
		work.cancel_work(e);
		itemHelper.unclaim_item_by_id(tag.soilId);
		return;
	}

	mov.path = path->path;
	tag.step = FarmSoilTag::GOTO_SOIL;
}

void gotoSoil(const Entity & e, MovementComponent &mov, const Coordinates & co, WorkTemplate<FarmSoilTag>& work, FarmSoilTag & tag)
{
	work.followPath(mov, co, tag.soilCo, [&e, &work]()
	{
		// On failed pathing
		work.cancel_work(e);
		return;
	}, [&e, &work, &tag]
	{
		world.emit(pickup_item_message{ SLOT_CARRYING, e.getId().index, tag.soilId, 0 });
		tag.step = FarmSoilTag::FIND_FARM;
		return;
	});
}

void findFarm(const Entity & e, MovementComponent &mov, const Coordinates & co, WorkTemplate<FarmSoilTag>& work, FarmSoilTag & tag)
{
	// Find closest farm tiles that need soil
	std::map<int, int> farmsDist;
	for (const auto& f : designations->farming)
	{
		if (f.second.step != FarmInfo::ADD_SOIL)
			continue;

		auto dist = get_3D_distance(co, idxToCo(f.first));

		farmsDist.insert(std::make_pair(static_cast<int>(dist), f.first));
	}

	// Loop through closest destinations first
	// attempting to find a path
	for (const auto& f : farmsDist)
	{
		auto path = findPath(co, idxToCo(f.second));

		if (!path->failed)
		{
			mov.path = path->path;
			tag.farmCo = idxToCo(f.second);
			tag.step = FarmSoilTag::GOTO_FARM;
			return;
		}
	}

	// No path found ( or no farms )
	// OR slim chance there are identically distant farms with a path,
	// and some without a path
	work.cancel_work(e);
	world.emit(drop_item_message{ SLOT_CARRYING, e.getId().index, tag.soilId, co });
	return;
}

void gotoFarm(const Entity & e, MovementComponent &mov, const Coordinates & co, WorkTemplate<FarmSoilTag>& work, FarmSoilTag & tag)
{
	work.followPath(mov, co, tag.farmCo, [&work, &e]()
	{
		// On path failure
		work.cancel_work(e);
		return;
	}, [&tag]
	{
		// On reaching the farm
		tag.step = FarmSoilTag::SPREAD_SOIL;
		return;
	});
}

void spreadSoil(const Entity & e, const double& duration, MovementComponent &mov, const Coordinates & co, WorkTemplate<FarmSoilTag>& work, FarmSoilTag & tag)
{
	constexpr double baseTime = 6000.0;

	auto farmFind = designations->farming.find(getIdx(co));

	if (farmFind == designations->farming.end())
	{
		world.emit(drop_item_message{ SLOT_CARRYING, e.getId().index, tag.soilId, co });
		work.cancel_work(e);
		return;
	}

	auto& stats = e.getComponent<Stats>();

	// Let's do the work 
	if (farmFind->second.progress < baseTime)
	{
		doWorkTime(stats, jobSkill, duration, farmFind->second.progress);
		return;
	}

	// Work time is all done!

	auto& soilE = world.getEntity(tag.soilId);
	auto& soilI = soilE.getComponent<Item>();

	region::setMaterial(co, soilI.material);

	soilE.kill(); 

	farmFind->second.step = FarmInfo::PLANT;
	farmFind->second.progress = 0.0;

	work.cancel_work(e);
	return;
}
