#include "stdafx.h"
#include "Map\Tile.h"
#include "FarmingClearAi.h"
#include "WorkTemplate.h"
#include "Designations\Designations.h"
#include "Raws\raws.h"
#include "Raws\Defs\ItemDefs.h"
#include "Raws\ReadPlants.h"
#include "Raws\Defs\PlantDef.h"
#include "Raws\ItemRead.h"
#include "Raws\Defs\ItemDefs.h"
#include "Raws\Materials.h"
#include "Raws\Defs\MaterialDef.h"
#include "Helpers\Rng.h"
#include "JobBoard.h"
#include "ECS\Components\Seed.h"
#include "ECS\Components\Sentients\Stats.h"
#include "ECS\Systems\helpers\PathFinding.h"
#include "ECS\Messages\hoe_map_changed_message.h"
#include "ECS\Systems\DijkstraSystems\DijkstraMapsHandler.h"

static const std::string jobSkill = "farming";

namespace JobsBoard
{
	void evaluate_farm_clearing(JobBoard & board, const Entity & e, AiWorkComponent &prefs, const Coordinates& co, JobEvaluatorBase * jt)
	{
		if (designations->farming.empty())
			return;

		// Find numerical job rating value for this type of work
		auto pfind = prefs.jobPrefrences.find(jobSkill);

		if (pfind == prefs.jobPrefrences.end() || pfind->second < 1)
			return;

		for (const auto& f : designations->farming)
		{
			if (f.second.step == FarmInfo::CLEAR && !f.second.beingWorked)
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

inline void testTool(const Entity &e, FarmClearTag &tag, WorkTemplate<FarmClearTag> &work)
{
	if (work.hasCorrectTool(e, TOOL_FARMING))
		tag.step = FarmClearTag::FIND_FARM;
	else
		tag.step = FarmClearTag::FIND_HOE;
}

inline void findHoe(const Entity &e, const Coordinates& co, WorkTemplate<FarmClearTag> &work, FarmClearTag &tag)
{
	work.followMap(hoe_map, e, co, [&e, &work]()
	{
		// On not being able to reach how
		work.cancel_work(e);
		return;
	}, [&work, &tag, &e, &co]
	{
		// On findin the hoe
		work.pickup_tool<hoe_map_changed_message>(e, co, TOOL_FARMING, tag.toolId, [&work, &tag, &e]()
		{
			// On failed pickup
			work.cancel_work(e);
			return;
		}, [&tag]
		{
			// On success
			tag.step = FarmClearTag::FIND_FARM;
			return;
		});
	});
}

inline void findFarm(const Entity &e, const Coordinates& co, WorkTemplate<FarmClearTag> &work, FarmClearTag &tag)
{
	// Distance and idx of possible targest
	std::map<int, Coordinates> targets;
	for (const auto& f : designations->farming)
	{
		// Farm is in clearing stage and is not currently being worked
		if (f.second.step == FarmInfo::CLEAR && f.second.progress == 0.0)
		{
			auto farmCo = idxToCo(f.first);
			auto dist = get_3D_distance(co, farmCo);

			targets.insert(std::make_pair(static_cast<int>(dist), farmCo));
		}
	}

	// Find closest possible path to a farm that needs clearing
	for (const auto& t : targets)
	{
		auto path = findPath(co, t.second);

		if (!path->failed)
		{
			tag.dest = getIdx(t.second);
			tag.step = FarmClearTag::GOTO_FARM;

			designations->farming[tag.dest].beingWorked = true;
			e.getComponent<MovementComponent>().path = path->path; // Need to make movement path into a unique ptr so that we're not copying the entire path!
			return;
		}
	}

	// On failed pathfinding
	work.cancel_work(e);
	return;
}

inline void gotoFarm(const Entity &e, const Coordinates& co, WorkTemplate<FarmClearTag> &work, FarmClearTag &tag)
{
	auto& mov = e.getComponent<MovementComponent>();

	work.followPath(mov, co, idxToCo(tag.dest), [&work, &tag, &e]()
	{
		// On not finding a path
		designations->farming[tag.dest].beingWorked = false;
		tag.step = FarmClearTag::FIND_FARM;
		return;
	}, [&tag]
	{
		// On success
		tag.step = FarmClearTag::CLEAR_AREA;
		return;
	});
}

inline void clearArea(const Entity &e, const Coordinates& co, WorkTemplate<FarmClearTag> &work, FarmClearTag &tag, const double& duration)
{
	constexpr int baseDifficulty = 10;
	constexpr double defaultClearTime = 2400.0;

	auto idx = getIdx(co);

	auto ffind = designations->farming.find(idx);

	// Invalid designation spot
	if (ffind == designations->farming.end())
	{
		tag.step = FarmClearTag::FIND_FARM;

		auto failFind = designations->farming.find(tag.dest);

		if(failFind != designations->farming.end())
			failFind->second.beingWorked = false;

		return;
	}

	auto& stats = e.getComponent<Stats>();

	int plantType = region::plantType(idx);

	// Either use the plant harvest time or the default
	const double time = plantType > 0 ? getPlantDef(plantType)->time.second : defaultClearTime;

	if (ffind->second.progress < time)
	{
		doWorkTime(stats, jobSkill, duration, ffind->second.progress);
		return;
	}

	int difficulty = baseDifficulty;
	if (plantType > 0)
	{
		auto plant = getPlantDef(plantType); 

		const auto produce = plant->harvestsTo[region::plantLifeCycle(idx)];

		if (produce != "none")
		{
			std::string mat = "organic";
			auto item = getItemDef(produce);

			if (item != nullptr)
			{
				if (item->categories.test(ITEM_FOOD)) mat = "food";
			}

			auto quality = calculateQuality(stats, jobSkill, difficulty);

			auto sitem = spawnItemOnGround(produce, getMaterialIdx(mat), co, SpawnColor::ITEM_COLOR, quality);
			sitem.getComponent<Item>().name = plant->name;

			// Produce seeds
			// Only if plant produces something at this growth stage
			spawnSeeds(rng.range(1, 4), co, plant->tag);
		}
		
		difficulty = plant->difficulty;
	}

	// Clear tile of all plants
	region::setPlantType(idx, 0);
	region::setPlantHealth(idx, 0);
	region::setPlantLifecycle(idx, 0);
	region::setPlantTicker(idx, 0);
	region::tile_recalc(idxToCo(idx));

	// Check if we need to add soil to farm or if plot already has it

	const auto tileMat = getMaterial(region::getTileMaterial(co));

	if (tileMat && (tileMat->matType == MAT_SOIL || tileMat->matType == MAT_SAND))
		ffind->second.step = FarmInfo::PLANT;

	else
		ffind->second.step = FarmInfo::ADD_SOIL;

	// Farm plot is no longer being worked
	ffind->second.progress = 0.0;
	ffind->second.beingWorked = false;

	giveWorkXp(stats, jobSkill, difficulty);

	// Done
	work.cancel_work(e);
}

void FarmingClearAi::init()
{
	JobsBoard::register_job_offer<FarmClearTag>(JobsBoard::evaluate_farm_clearing);
}

void FarmingClearAi::update(const double duration)
{
	for (const auto& e : getEntities())
	{
		WorkTemplate<FarmClearTag> work;

		auto& co = e.getComponent<PositionComponent>().co;
		auto& tag = e.getComponent<FarmClearTag>();

		switch (tag.step)
		{
		case FarmClearTag::TEST_TOOL:
			testTool(e, tag, work);
			break;

		case FarmClearTag::FIND_HOE:
			findHoe(e, co, work, tag);
			break;

		case FarmClearTag::FIND_FARM:
			findFarm(e, co, work, tag);
			break;

		case FarmClearTag::GOTO_FARM:
			gotoFarm(e, co, work, tag);
			break;

		case FarmClearTag::CLEAR_AREA:
			clearArea(e, co, work, tag, duration);
			break;
		}
	}
}
