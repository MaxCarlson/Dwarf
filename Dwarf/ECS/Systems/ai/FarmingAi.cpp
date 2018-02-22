#include "stdafx.h"
#include "FarmingAi.h"
#include "JobBoard.h"
#include "Map\Tile.h"
#include "WorkTemplate.h"
#include "Designations\Designations.h"
#include "Raws\ReadPlants.h"
#include "Raws\Defs\PlantDef.h"
#include "ECS\Components\Sentients\Stats.h"
#include "../DijkstraSystems/DijkstraMapsHandler.h"
#include "ECS\Messages\harvest_map_changed_message.h"
#include "ECS\Messages\drop_item_message.h"
#include "ECS\Systems\helpers\SeedsHelper.h"
#include "ECS\Systems\helpers\PathFinding.h"
#include "ECS\Systems\helpers\ItemHelper.h"
#include "Raws\Defs\ItemDefs.h"

static const std::string jobSkill = "farming";


namespace JobsBoard
{
	void evaluate_farming(JobBoard & board, const Entity & e, AiWorkComponent &prefs, const Coordinates& co, JobEvaluatorBase * jt)
	{
		if (designations->farming.empty())
			return;

		// Find numerical job rating value for this type of work
		auto pfind = prefs.jobPrefrences.find(jobSkill);

		if (pfind->second < 1 || pfind == prefs.jobPrefrences.end())
			return;

		auto find = board.find(pfind->second);

		for (const auto& f : designations->farming)
		{
			if (f.second.step == FarmInfo::PLANT && !f.second.beingWorked)
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
//void findFarm(const Entity& e, MovementComponent &mov, WorkTemplate<PlantingTag> &work, const Coordinates &co, PlantingTag &tag);
void gotoFarm(const Entity& e, MovementComponent &mov, WorkTemplate<PlantingTag> &work, const Coordinates &co, PlantingTag &tag);
void doPlanting(const Entity& e, MovementComponent &mov, WorkTemplate<PlantingTag> &work, const Coordinates &co, PlantingTag &tag, const double& duration);


inline void updateHarvest()
{
	bool changed = false;
	region::forFarmPlots([&changed](const int idx)
	{
		auto plantT = region::plantType(idx);
		auto lfc = region::plantLifeCycle(idx);

		auto* plant = getPlantDef(plantT);

		if (plant == nullptr || !plantT)
			return;

		if (plant->harvestsTo[lfc] != "none")
		{
			changed = true;
			designations->harvest.emplace(idx);
		}
	});

	if (changed)
		world.emit(harvest_map_changed_message {});
}

void FarmingAi::update(const double duration)
{
	constexpr double harvestUpdateSpeed = 2000.0;
	static double harvestTimer = 0.0;
	harvestTimer += duration;

	// Auto add any farm plots that are ready to harvest 
	// to the harvest designations
	if (harvestTimer > harvestUpdateSpeed)
	{
		updateHarvest();
		harvestTimer = 0.0;
	}

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

		case PlantingTag::GOTO_FARM:
			gotoFarm(e, mov, work, co, tag);
			break;

		case PlantingTag::PLANT:
			doPlanting(e, mov, work, co, tag, duration);
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
		tag.step = PlantingTag::FIND_HOE;
	}, [&e, &tag]
	{
		tag.step = PlantingTag::FIND_SEEDS;
		world.emit(pickup_item_message{ SLOT_TOOL, e.getId().index, tag.itemId, 0 });
	});
}

void findSeeds(const Entity & e, MovementComponent &mov, WorkTemplate<PlantingTag> &work, const Coordinates & co, PlantingTag & tag)
{
	// Build a map of closest farms that match planting status and aren't being worked
	std::map<int, std::pair<FarmInfo, Coordinates>> farms;
	for (const auto& f : designations->farming)
	{
		if (f.second.step == FarmInfo::PLANT && !f.second.beingWorked)
		{
			auto dist = get_3D_distance(co, idxToCo(f.first));

			farms[dist] = std::make_pair(f.second, idxToCo(f.first));
		}
	}

	// Find seeds 
	// and try to find a path to them
	for (const auto& s : farms)
	{
		const auto sId = s.second.first.seedId;

		// Farm designation already has a seed claimed
		if (sId > 0)
		{
			auto seed = world.getEntity(sId);

			if (!seed.hasComponent<PositionComponent>())
				continue;

			const auto& seedCo = seed.getComponent<PositionComponent>().co;

			auto path = findPath(co, seedCo);

			if (!path->failed)
			{
				mov.path = path->path;
				tag.targetCo = seedCo;
				tag.farmCo = s.second.second;
				tag.itemId = sId;

				// Mark farm as unavailible to other workers
				designations->farming[getIdx(tag.farmCo)].beingWorked = true;

				tag.step = PlantingTag::GOTO_SEEDS;
				return;
			}
		}

		// Farming designation has no seed claimed
		// find one and claim it
		else
		{
			seedsHelper.forAllSeeds([&co, &tag, &mov, &s, &sId](Entity e)
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
					tag.farmCo = s.second.second;
					tag.itemId = sId;

					// Mark farm as unavailible to other workers
					designations->farming[getIdx(tag.farmCo)].beingWorked = true;

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
	bool fail = false;
	work.followPath(mov, co, tag.targetCo, [&tag, &fail]()
	{
		// On pathing failure
		fail = true;

	}, [&co, &tag, &e, &mov, &fail]
	{
		auto path = findPath(co, tag.farmCo);

		if (path->failed)
		{
			fail = true;
			return;
		}

		mov.path = path->path;
		tag.step = PlantingTag::GOTO_FARM;
		world.emit(pickup_item_message{ SLOT_CARRYING, e.getId().index, tag.itemId, 0 });
	});

	if (fail)
	{
		tag.step = PlantingTag::FIND_SEEDS;

		auto find = designations->farming.find(getIdx(tag.farmCo));
		if (find != designations->farming.end())
			find->second.beingWorked = false;
	}
}

void gotoFarm(const Entity & e, MovementComponent & mov, WorkTemplate<PlantingTag>& work, const Coordinates & co, PlantingTag & tag)
{
	work.followPath(mov, co, tag.farmCo, [&tag, &co, &mov, &e]()
	{
		auto failFind = designations->farming.find(getIdx(tag.farmCo));

		if (failFind != designations->farming.end())
			failFind->second.beingWorked = false;

		// On pathing failure
		// Find a farm that can take the seeds we hold
		std::map<double, Coordinates> fMap;
		const auto& seedTag = world.getEntity(tag.itemId).getComponent<Seed>().plantTag;
		for (const auto& f : designations->farming)
		{
			if (f.second.step == FarmInfo::PLANT && !f.second.beingWorked && f.second.seedType == seedTag)
			{
				fMap.insert(std::make_pair(get_3D_distance(co, idxToCo(f.first)), idxToCo(f.first)));
			}
		}

		for (const auto& d : fMap)
		{
			auto path = findPath(co, d.second);

			if (!path->failed)
			{
				mov.path = path->path;
				tag.farmCo = d.second;
				return;
			}
		}

		// Cannot find path or cannot find farm to take our seeds
		// Drop seeds and find different ones
		tag.step = PlantingTag::FIND_SEEDS;
		tag.farmCo = EMPTY_COORDINATES;
		
		world.emit(drop_item_message{ SLOT_CARRYING, e.getId().index, tag.itemId, co });

	}, [&tag]
	{
		// On reaching farm
		tag.step = PlantingTag::PLANT;
	});
}

void doPlanting(const Entity & e, MovementComponent & mov, WorkTemplate<PlantingTag>& work, const Coordinates & co, PlantingTag & tag, const double& duration)
{
	const auto idx = getIdx(co);
	auto farmFind = designations->farming.find(idx);

	if (farmFind == designations->farming.end())
	{
		tag.step = PlantingTag::GOTO_FARM; // This will also drop and find different seeds if it cannot find the farm
		return;
	}

	const auto plant = getPlantDef(getPlantIdx(farmFind->second.seedType));
	auto& stats = e.getComponent<Stats>();

	if (farmFind->second.progress < plant->time.first)
	{
		doWorkTime(stats, jobSkill, duration, farmFind->second.progress);
	}

	// Done with work time!

	deleteItemFromInventory(e, e.getComponent<Inventory>(), SLOT_CARRYING); // Make Entities be able to hold more than one seed!

	// Set plant into tile
	region::setFarmPlot(idx);
	region::setPlantHealth(idx, 10);
	region::setPlantLifecycle(idx, 0);
	region::setPlantTicker(idx, 0);
	region::setPlantType(idx, getPlantIdx(farmFind->second.seedType));
	region::tile_recalc(idxToCo(idx));

	farmFind->second.progress = 0.0;
	farmFind->second.beingWorked = false;
	farmFind->second.step = FarmInfo::GROWING;

	giveWorkXp(stats, jobSkill, plant->difficulty);
	
	work.cancel_work(e);
}
