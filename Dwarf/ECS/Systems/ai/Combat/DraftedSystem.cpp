#include "stdafx.h"
#include "DraftedSystem.h"
#include "CombatTemplate.h"
#include "ECS\Systems\ai\WorkTemplate.h"

void DraftedSystem::init()
{
}

inline void defendArea(const Entity &e, const Coordinates &co, Drafted &tag, CombatTemplate<Drafted> &combat, MovementComponent &mov);
inline void gotoDest(const Entity &e, const Coordinates &co, Drafted &tag, CombatTemplate<Drafted> &combat, MovementComponent &mov);
inline void gotoTarget(const Entity &e, const Coordinates &co, Drafted &tag, CombatTemplate<Drafted> &combat, MovementComponent &mov);
inline void attackTarget(const Entity &e, const Coordinates &co, Drafted &tag, CombatTemplate<Drafted> &combat, MovementComponent &mov, const double &duration);


void DraftedSystem::update(const double duration) // TODO: Messages when drafted pawns are reaching low happiness / needs!
{
	for (const auto& e : getEntities())
	{
		CombatTemplate<Drafted> combat; 

		Drafted &tag = e.getComponent<Drafted>();
		const Coordinates &co = e.getComponent<PositionComponent>().co;
		MovementComponent &mov = e.getComponent<MovementComponent>();

		switch (tag.step)
		{
		case Drafted::DEFEND: // TODO: Add in entity view and attack enemies close enough. TODO: Add in return to area after combat. TODO:
			defendArea(e, co, tag, combat, mov);
			break;
		case Drafted::GOTO_DEST:
			gotoDest(e, co, tag, combat, mov);
			break;
		case Drafted::GOTO_TARGET:
			gotoTarget(e, co, tag, combat, mov);
			break;
		case Drafted::ATTACK:
			attackTarget(e, co, tag, combat, mov, duration);
			break;
		}
	}
}

inline void defendArea(const Entity & e, const Coordinates & co, Drafted & tag, CombatTemplate<Drafted>& combat, MovementComponent & mov)
{
	// TODO: Add in entity view and attack enemies close enough.
	// TODO: Add in return to area after combat.
	// TODO:
}

inline void gotoDest(const Entity & e, const Coordinates & co, Drafted & tag, CombatTemplate<Drafted>& combat, MovementComponent & mov)
{
	WorkTemplate<Drafted> work;

	work.followPath(mov, co, tag.targetCo, [&tag]()
	{
		tag.step = Drafted::DEFEND; // TODO: Message about pathing failure?
	}, [&tag]
	{
		tag.step = Drafted::DEFEND;
	});
}

inline void gotoTarget(const Entity & e, const Coordinates & co, Drafted & tag, CombatTemplate<Drafted>& combat, MovementComponent & mov)
{
	constexpr double distance = 1.44;

	// TODO: Change distance for Entities with ranged weapons

	combat.getCloseToMovingThing(e, mov, co, tag.targetId, distance, [&tag]()
	{
		// On pathing or id failure
		tag.step = Drafted::DEFEND;

	}, [&tag]
	{
		const auto& target = world.getEntity(tag.targetId);

		if (!target.isValid() || !target.hasComponent<PositionComponent>())
		{
			tag.step = Drafted::DEFEND;
			return;
		}

		tag.step = Drafted::ATTACK;
		// Store position component of target
		// so we know if target moves
		tag.targetCo = target.getComponent<PositionComponent>().co;
	});
}

inline void attackTarget(const Entity & e, const Coordinates & co, Drafted & tag, CombatTemplate<Drafted>& combat, MovementComponent & mov, const double &duration)
{
	const auto& target = world.getEntity(tag.targetId); // Reading from id may be problematic if entity is killed an instantly recreated as something else ~~ May cause issues

	if (!target.isValid() || !target.hasComponent<PositionComponent>()) 
	{
		tag.step = Drafted::DEFEND;
		return;
	}

	auto& cBase = e.getComponent<CombatBase>(); 

	const auto& targetCo = target.getComponent<PositionComponent>().co;

	combat.attackEntity(duration, e, cBase, target, [&tag]()
	{
		// No longer close enough to attack
		tag.step = Drafted::GOTO_TARGET;
	});
}
