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
				auto distance = get_3D_distance(co, idxToCo(f.first));
				// Overwrite if distance to equally prefered job is less
				// or add if job preference doesn't exist
				if (find->second.distance > distance || find == board.end())
					board[pfind->second] = JobRating{ distance, jt };
			}
		}
	}
}

void FarmingAi::init()
{
}

void FarmingAi::update(const double duration)
{
	for (auto e : getEntities())
		doWork(e, duration);
}

void findSeeds(const Coordinates& co, PlantingTag &tag)
{
	seedsHelper.forAllUnclaimedSeeds([&tag](Entity s)
	{
		
	});
}

void FarmingAi::doWork(Entity & e, const double& duration)
{
	WorkTemplate<PlantingTag> work;

	auto& co = e.getComponent<PositionComponent>().co;
	auto& tag = e.getComponent<PlantingTag>();

	switch (tag.step)
	{
	case PlantingTag::FIND_SEEDS:
		findSeeds(co, tag);
		break;

	case PlantingTag::GOTO_SEEDS:

		break;

	case PlantingTag::FIND_PLANTING:

		break;

	case PlantingTag::PLANT:

		break;
	}

	/*
	if (tag.step == PlantingTag::FIND_PLANTING)
	{
		work.followMap(harvest_map, e, co, [&e, &work]()
		{
			// On Failure
			work.cancel_work(e);
			return;
		}, [&e, &co, &tag]
		{
			// On finding harvest location
			tag.step = PlantingTag::PLANT;
			return;
		});
		return;
	}

	else if (tag.step == PlantingTag::PLANT)
	{
		const int idx = getIdx(co);

		// If this is the first time we've reached this step
		// Remove the planting designation we are on top of
		//
		// Also capture the plant def idx and store it so we 
		// know which type of plant we're planting
		if (tag.progress == 0.0)
		{
			bool found = false;
			designations->planting.erase(std::remove_if(
				designations->planting.begin(),
				designations->planting.end(),
				[&idx, &tag, &found](std::pair<int, size_t> pdes)
			{
				if (idx == pdes.first)
				{
					tag.plantDefIdx = pdes.second;
					found = true;
					return true;
				}
				return false;
			}),
				designations->planting.end());

			if (found)
				emit(planting_map_changed_message{});

			// Designation not found
			else
			{
				tag.step = PlantingTag::FIND_PLANTING;
				return;
			}
		}

		auto& stats = e.getComponent<Stats>();

		const auto plant = getPlantDef(tag.plantDefIdx);

		// Add time progress based on skill in farming and step duration
		doWorkTime(stats, jobSkill, duration, tag.progress);

		// Check progress against set time it takes to plant the plant
		if (tag.progress < plant->time.first)
			return;

		// Finalize planting

		giveWorkXp(stats, jobSkill, plant->difficulty);
		region::setFarmPlot(idx);
		region::setPlantType(idx, getPlantIdx(plant->tag));
		region::setPlantHealth(idx, 10);
		region::setPlantLifecycle(idx, 0);
		region::setPlantTicker(idx, 0);
		region::setFlag(co, region::Flag::CONSTRUCTION);

		region::tile_recalc(co);

		work.cancel_work(e);
		return;
	}
	*/
}

