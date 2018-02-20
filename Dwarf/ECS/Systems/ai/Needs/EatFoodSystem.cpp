#include "EatFoodSystem.h"
#include "../WorkTemplate.h"
#include "../JobBoard.h"

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

inline void findFood(const Entity & e, const Coordinates & co, EatFoodTag & tag, WorkTemplate<EatFoodTag>& work, MovementComponent & mov)
{
}

inline void gotoFood(const Entity & e, const Coordinates & co, EatFoodTag & tag, WorkTemplate<EatFoodTag>& work, MovementComponent & mov)
{
}

inline void findTable(const Entity & e, const Coordinates & co, EatFoodTag & tag, WorkTemplate<EatFoodTag>& work, MovementComponent & mov)
{
}

inline void gotoTable(const Entity & e, const Coordinates & co, EatFoodTag & tag, WorkTemplate<EatFoodTag>& work, MovementComponent & mov)
{
}

inline void eatFood(const Entity & e, const Coordinates & co, EatFoodTag & tag, WorkTemplate<EatFoodTag>& work, MovementComponent & mov)
{
}
