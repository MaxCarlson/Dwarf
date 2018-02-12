#include "stdafx.h"
#include "Map\Tile.h"
#include "FarmingClearAi.h"
#include "WorkTemplate.h"
#include "Designations.h"
#include "Raws\raws.h"
#include "Raws\Defs\ItemDefs.h"
#include "Raws\ReadPlants.h"
#include "Raws\Defs\PlantDef.h"
#include "Raws\ItemRead.h"
#include "Raws\Defs\ItemDefs.h"
#include "Raws\Materials.h"
#include "Helpers\Rng.h"
#include "ECS\Components\Seed.h"
#include "ECS\Systems\helpers\PathFinding.h"
#include "ECS\Messages\hoe_map_changed_message.h"
#include "ECS\Systems\DijkstraSystems\DijkstraMapsHandler.h"


inline void testTool(const Entity &e, FarmClearTag &tag, WorkTemplate<FarmClearTag> &work)
{
	if (work.hasCorrectTool(e, TOOL_FARMING))
		tag.step = FarmClearTag::GOTO_FARM;
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
			tag.step = FarmClearTag::GOTO_FARM;
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
		if (f.second.step == FarmInfo::CLEAR)
		{
			auto farmCo = idxToCo(f.first);
			auto dist = get_3D_distance(co, farmCo);

			targets.insert(std::make_pair(static_cast<int>(dist), farmCo));
		}
	}

	for (const auto& t : targets)
	{
		auto path = findPath(co, t.second);

		if (!path->failed)
		{
			tag.dest = getIdx(t.second);
			tag.step = FarmClearTag::GOTO_FARM;
			e.getComponent<MovementComponent>().path = path->path; // Need to make movement path into a unique ptr so that we're not copying the entire path!
			return;
		}
	}

	// On failed pathfinding
	work.cancel_work(e);
}

inline void gotoFarm(const Entity &e, const Coordinates& co, WorkTemplate<FarmClearTag> &work, FarmClearTag &tag)
{
	auto& mov = e.getComponent<MovementComponent>();

	work.followPath(mov, co, idxToCo(tag.dest), [&work, &tag, &e]()
	{
		// On not finding a path
		work.cancel_work(e);
		return;
	}, [&tag]
	{
		// On success
		tag.step = FarmClearTag::CLEAR_AREA;
		return;
	});
}

inline void clearArea(const Entity &e, const Coordinates& co, WorkTemplate<FarmClearTag> &work, FarmClearTag &tag)
{
	auto idx = getIdx(co);

	auto ffind = designations->farming.find(idx);

	// Invalid designation spot
	if (ffind == designations->farming.end())
	{
		work.cancel_work(e);
		return;
	}

	if (region::plantType(idx) > 0)
	{
		auto plant = getPlantDef(region::plantType(idx));

		const auto produce = plant->harvestsTo[region::plantLifeCycle(idx)];

		if (produce != "none")
		{
			std::string mat = "organic";
			auto item = getItemDef(produce);

			if (item != nullptr)
			{
				if (item->categories.test(ITEM_FOOD)) mat = "food";
			}
			auto sitem = spawnItemOnGround(produce, getMaterialIdx(mat), co, SpawnColor::ITEM_COLOR);
			sitem.getComponent<Item>().name = plant->name;
		}

		// Produce seeds
		spawnSeeds(rng.range(1, 4), co, plant->tag);


		// Clear tile of all plants
		region::setPlantType(idx, 0);
		region::setPlantHealth(idx, 0);
		region::setPlantLifecycle(idx, 0);
		region::setPlantTicker(idx, 0);

		// Check if we need to add soil to farm or if plot already has it

		const auto tileMat = getMaterial(region::getTileMaterial(co));
	}
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

		case FarmClearTag::GOTO_FARM:
			gotoFarm(e, co, work, tag);
			break;

		case FarmClearTag::CLEAR_AREA:
			clearArea(e, co, work, tag);
			break;
		}
	}
}
