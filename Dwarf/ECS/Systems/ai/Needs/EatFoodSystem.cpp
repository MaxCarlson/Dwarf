#include "EatFoodSystem.h"
#include "../WorkTemplate.h"
#include "../JobBoard.h"
#include "ECS\Systems\helpers\ItemHelper.h"
#include "Raws\ItemRead.h"
#include "Raws\Defs\ItemDefs.h"
#include "ECS\Systems\helpers\PathFinding.h"
#include "ECS\Systems\helpers\BuildingHelper.h"

namespace JobsBoard
{
	void evaluate_eating(JobBoard & board, const Entity & e, AiWorkComponent &prefs, const Coordinates& co, JobEvaluatorBase * jt)
	{
		const auto& hunger = e.getComponent<Needs>().needs[static_cast<int>(NeedIdx::HUNGER)];
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
inline void eatFood(const Entity &e, const Coordinates& co, EatFoodTag &tag, WorkTemplate<EatFoodTag> &work, MovementComponent &mov);


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
			eatFood(e, co, tag, work, mov);
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
	// Try and find a table with a chair adjacent
	// TODO: Cache these when they're built in an unordered_map

	// If not try to find a chair or a table ~ Minor unhappy thoughts
	std::map<int, size_t> distance;
	buildingHelper.forEachBuildingOfType<PROVIDES_CHAIR | PROVIDES_TABLE>([&tag, &co, &distance](const Entity &b)
	{
		if (!b.hasComponent<PositionComponent>())
			return;

		auto dist = static_cast<int>(get_3D_distance(co, b.getComponent<PositionComponent>().co)); 

		distance.emplace(dist, b.getId());
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
			tag.step = EatFoodTag::GOTO_TABLE;
			tag.tableId = d.second;
			tag.targetCo = bco;
			return;
		}
	}
}

inline void gotoTable(const Entity & e, const Coordinates & co, EatFoodTag & tag, WorkTemplate<EatFoodTag>& work, MovementComponent & mov)
{
}

inline void eatFood(const Entity & e, const Coordinates & co, EatFoodTag & tag, WorkTemplate<EatFoodTag>& work, MovementComponent & mov)
{
}
