#include "stdafx.h"
#include "HarvestAi.h"
#include "WorkTemplate.h"
#include "JobBoard.h"
#include "Designations.h" 
#include "Map\Tile.h"
#include "Raws\ReadPlants.h"
#include "Raws\Defs\PlantDef.h"
#include "Raws\Defs\ItemDefs.h"
#include "Raws\ItemRead.h"
#include "Raws\raws.h"
#include "Raws\Materials.h"
#include "Raws\Defs\MaterialDef.h"
#include "../../Components/Sentients/Stats.h"
#include "../../Messages/harvest_map_changed_message.h"

namespace JobsBoard
{
	void evaluate_harvest(JobBoard & board, const Entity & e, Coordinates co, JobEvaluatorBase * jt)
	{
		if (designations->harvest.empty())
			return;

		auto distance = harvest_map.get(getIdx(co));

		if (distance > MAX_DIJKSTRA_DISTANCE - 1)
			return;

		board.insert(std::make_pair(distance, jt));
	}
}

void HarvestAi::init()
{
	JobsBoard::register_job_offer<HarvestTag>(JobsBoard::evaluate_harvest);
}

void HarvestAi::update(const double duration)
{
	for (auto& e : getEntities())
		doHarvest(e, duration);
}

void HarvestAi::doHarvest(const Entity& e, const double& duration)
{
	WorkTemplate<HarvestTag> work;

	auto& co  = e.getComponent<PositionComponent>().co;
	auto& tag = e.getComponent<HarvestTag>();

	if (tag.step == HarvestTag::FIND_HARVEST)
	{
		work.followMap(harvest_map, e, co, [&e, &work]()
		{
			// On Failure
			work.cancel_work(e);
			return;
		}, [&e, &co, &tag]
		{
			// On finding harvest location
			tag.step = HarvestTag::HARVEST;
			return;
		});
		return;
	}

	else if (tag.step == HarvestTag::HARVEST) // Currently harvesting isn't bound by plant difficulty aside from harvest time. Figure out if we want to change that
	{
		const int idx = getIdx(co);

		designations->harvest.erase(std::remove_if(
			designations->harvest.begin(), 
			designations->harvest.end(),
			[&idx](auto d) { return idx == getIdx(d.second);

		}), designations->harvest.end());

		if (region::plantType(idx) == 0)
		{
			work.cancel_work(e);
			return;
		}

		static const std::string skillName = "farming";

		auto& stats = e.getComponent<Stats>();
		const auto  plant = getPlantDef(region::plantType(idx));

		doWorkTime(stats, skillName, duration, tag.progress);

		if (tag.progress < plant->time.second)
			return;

		const auto produce = plant->harvestsTo[region::plantLifeCycle(idx)];

		if (produce != "none")
		{
			std::string type = "organic";

			auto find = getItemDef(produce);

			if (find)
			{
				if (find->categories.test(ITEM_FOOD)) type = "food";

				auto item = spawnItemOnGround(produce, getMaterialIdx(type), co, SpawnColor::ITEM_COLOR);  

				// Spawn seeds
				spawnItemOnGround("seed", getMaterialIdx("organic"), co, SpawnColor::CUSTOM_COLOR, getItemDef(produce)->ch.fg);
			}

			giveWorkXp(stats, skillName, plant->difficulty);

			region::setPlantTicker(idx, 0);
			region::setPlantLifecycle(idx, 0);
			emit(harvest_map_changed_message{});
		}
		else
		{
			designations->harvest.erase(std::remove_if(
				designations->harvest.begin(),
				designations->harvest.end(),
				[&idx](auto d) { return idx == getIdx(d.second);

			}), designations->harvest.end());
		}

		work.cancel_work(e);
		return;
	}
}