#include "EatFoodSystem.h"
#include "../WorkTemplate.h"
#include "../JobBoard.h"
#include "ECS\Systems\helpers\ItemHelper.h"
#include "Raws\ItemRead.h"
#include "Raws\Defs\ItemDefs.h"
#include "ECS\Systems\helpers\PathFinding.h"
#include "ECS\Systems\helpers\BuildingHelper.h"
#include "ECS\Components\Quality.h"

namespace JobsBoard
{
	void evaluate_eating(JobBoard & board, const Entity & e, AiWorkComponent &prefs, const Coordinates& co, JobEvaluatorBase * jt)
	{
		if (!itemHelper.isItemCatagoryAvailible<ITEM_FOOD>()) // TODO: Make this check in entities inventory, And possibly in others inventories if need is bad enough?
			return;
	
		const auto& hunger = e.getComponent<Needs>().needs[static_cast<int>(NeedIdx::HUNGER)].lvl;
		const auto& thirst = e.getComponent<Needs>().needs[static_cast<int>(NeedIdx::THIRST)].lvl;

		int priority = 0;

		if (hunger > HungerThreshold::HUNGRY)
			priority = HungerPriority::FULL;

		else if (hunger > HungerThreshold::FAMISHED)
			priority = HungerPriority::HUNGRY;

		else if (hunger > HungerThreshold::URGENTLY)
			priority = HungerPriority::FAMISHED;

		else if (hunger > HungerThreshold::STARVING)
			priority = HungerPriority::URGENTLY_HUNGRY;
		else
			priority = HungerPriority::STARVING;

		if (thirst < ThirstThreshold::VERY_THRISTY) // Try and get some water out of food!
			priority = ThirstPriority::DEHYDRATED;

		// Don't eat when full
		if (priority == HungerPriority::FULL)
			return;

		// TODO: Add in closest food? Probably not really needed?
		int distance = 0;
		board.emplace(std::make_pair(priority, JobsBoard::JobRating { distance, jt }));
	}
}

void EatFoodSystem::init()
{
	JobsBoard::register_job_offer<EatFoodTag>(JobsBoard::evaluate_eating);
}

inline void findFood(const Entity &e, const Coordinates& co, EatFoodTag &tag, WorkTemplate<EatFoodTag> &work, MovementComponent &mov);
inline void gotoFood(const Entity &e, const Coordinates& co, EatFoodTag &tag, WorkTemplate<EatFoodTag> &work, MovementComponent &mov);
inline void findTable (const Entity &e, const Coordinates& co, EatFoodTag &tag, WorkTemplate<EatFoodTag> &work, MovementComponent &mov);
inline void gotoTable(const Entity &e, const Coordinates& co, EatFoodTag &tag, WorkTemplate<EatFoodTag> &work, MovementComponent &mov);
inline void eatFood(const Entity &e, const Coordinates& co, EatFoodTag &tag, WorkTemplate<EatFoodTag> &work, MovementComponent &mov, const double& duration);


void EatFoodSystem::update(const double duration)
{
	for (const auto& e : getEntities())
	{
		WorkTemplate<EatFoodTag> work;

		auto& co  = e.getComponent<PositionComponent>().co;
		auto& tag = e.getComponent<EatFoodTag>();
		auto& mov = e.getComponent<MovementComponent>();

		switch (tag.step)
		{
		case EatFoodTag::FIND_FOOD:
			findFood(e, co, tag, work, mov);
			break;

		case EatFoodTag::GOTO_FOOD:
			gotoFood(e, co, tag, work, mov);
			break;

		case EatFoodTag::FIND_TABLE:
			findTable(e, co, tag, work, mov);
			break;

		case EatFoodTag::GOTO_TABLE:
			gotoTable(e, co, tag, work, mov);
			break;

		case EatFoodTag::EAT_FOOD:
			eatFood(e, co, tag, work, mov, duration);
			break;
		}
	}
}

inline void findFood(const Entity & e, const Coordinates & co, EatFoodTag & tag, WorkTemplate<EatFoodTag>& work, MovementComponent & mov) // TODO: Create sepperate quality levels of food? RimWorld style? Prefrence off this
{
	// Figure out if we're already holding food or not!
	const auto foodSlot = e.getComponent<Inventory>().inventory[SLOT_FOOD];

	if (foodSlot) // We're holding food!
	{
		tag.step = EatFoodTag::FIND_TABLE;
		tag.foodId = foodSlot;
		return;
	}

	tag.foodId = itemHelper.findClosestItemTypeClaimIt(e, ITEM_FOOD, co, false); 

	if (tag.foodId == 0) // No food :(
	{
		work.cancel_work(e);
		return;
	}

	tag.targetCo = world.getEntity(tag.foodId).getComponent<PositionComponent>().co;

	auto path = findPath(co, tag.targetCo);

	if (path->failed)
	{
		work.cancel_work(e);
		return;
	}

	itemHelper.claim_item(world.getEntity(tag.foodId));
	mov.path = path->path;
	tag.step = EatFoodTag::GOTO_FOOD;
}

inline void gotoFood(const Entity & e, const Coordinates & co, EatFoodTag & tag, WorkTemplate<EatFoodTag>& work, MovementComponent & mov)
{
	work.followPath(mov, co, tag.targetCo, [&tag]()
	{
		// On pathing failure
		tag.step = EatFoodTag::FIND_FOOD;
		itemHelper.unclaim_item_by_id(tag.foodId);

	}, [&tag, &e]
	{
		tag.step = EatFoodTag::FIND_TABLE;
		world.emit(pickup_item_message { SLOT_FOOD, e.getId().index, tag.foodId, 0 });
	});
}

inline void findTable(const Entity & e, const Coordinates & co, EatFoodTag & tag, WorkTemplate<EatFoodTag>& work, MovementComponent & mov)
{
	// TODO: Try and find a table with a chair adjacent and make entities prefer this!
	// TODO: Cache these when they're built in an unordered_map

	// If not try to find a chair or a table ~ Minor unhappy thoughts
	std::map<int, size_t> distance;
	eachWith<Requires<Building>, Excludes<Claimed>>([&tag, &co, &distance](const Entity &b)
	{
		const auto& build = b.getComponent<Building>();

		if (build.provides.test(PROVIDES_CHAIR) || build.provides.test(PROVIDES_TABLE))
		{
			auto dist = static_cast<int>(get_3D_distance(co, b.getComponent<PositionComponent>().co));

			distance.emplace(dist, b.getId().index);
		}
	});

	if (distance.empty())
	{
		tag.step = EatFoodTag::EAT_FOOD;
		tag.tableStatus = EatFoodTag::TableChairStatus::NONE;
		return;
	}

	for (const auto& d : distance)
	{
		auto building = world.getEntity(d.second);
		const auto& bco = building.getComponent<PositionComponent>().co;

		auto path = findPath(co, bco);

		if (!path->failed)
		{
			bool chair = building.getComponent<Building>().provides.test(PROVIDES_CHAIR);

			tag.tableStatus = chair ? EatFoodTag::CHAIR : EatFoodTag::TABLE;
			tag.step = EatFoodTag::GOTO_TABLE;
			building.addComponent<Claimed>(e.getId().index);
			building.activate();
			tag.tableId = d.second;
			tag.targetCo = bco;
			return;
		}
	}
}

inline void gotoTable(const Entity & e, const Coordinates & co, EatFoodTag & tag, WorkTemplate<EatFoodTag>& work, MovementComponent & mov)
{
	work.followPath(mov, co, tag.targetCo, [&tag]()
	{
		tag.step = EatFoodTag::FIND_TABLE;
		world.getEntity(tag.tableId).removeComponent<Claimed>();
		world.getEntity(tag.tableId).activate();
	}, [&tag]
	{
		tag.step = EatFoodTag::EAT_FOOD;
	});
}

inline double foodQualityBenifit(const EatFoodTag &tag)
{
	const auto& foodItem = world.getEntity(tag.foodId);
	const auto* qual = &foodItem.getComponent<Quality>();

	if (qual == nullptr)
		return 1.0;

	switch (qual->quality)
	{
	case QualityDef::AWFUL_QUALITY:
		return 0.70;
	case QualityDef::POOR_QUALITY:
		return 0.85;
	case QualityDef::NORMAL_QUALITY:
		return 1.0;
	case QualityDef::GOOD_QUALITY:
		return 1.1;
	case QualityDef::SUPERIOR_QUALITY:
		return 1.2;
	case QualityDef::EPIC_QUALITY:
		return 1.3;
	case QualityDef::LEGENDAY_QUALITY:
		return 1.45;
	}
}

inline void eatFood(const Entity & e, const Coordinates & co, EatFoodTag & tag, WorkTemplate<EatFoodTag>& work, MovementComponent & mov, const double& duration)
{
	auto& hunger = e.getComponent<Needs>().needs[static_cast<int>(NeedIdx::HUNGER)];
	auto& thirst = e.getComponent<Needs>().needs[static_cast<int>(NeedIdx::THIRST)];

	// Improve satiation rate from higher quality foods
	if (tag.qualityMultiplier == 0.0)
		tag.qualityMultiplier = foodQualityBenifit(tag);

	constexpr double EAT_TIME = 15000.0;
	constexpr double GIVE_650_SATIATION_IN_15 = 153.8935;

	hunger.lvl += (duration /  GIVE_650_SATIATION_IN_15     ) * tag.qualityMultiplier;
	thirst.lvl += (duration / (GIVE_650_SATIATION_IN_15 * 8)) * tag.qualityMultiplier; // Improve thirst slightly

	tag.time += duration;

	if (tag.time > EAT_TIME || hunger.lvl > 1000.0)
	{
		world.getEntity(tag.foodId).kill();
		e.getComponent<Inventory>().inventory[SLOT_FOOD] = 0;
		work.cancel_work(e);
		return;
	}
}
