#include "stdafx.h"
#include "DraftedSystem.h"
#include "CombatTemplate.h"

void DraftedSystem::init()
{
}

inline void defendArea(const Entity &e, const Coordinates &co, Drafted &tag, CombatTemplate<Drafted> &combat, MovementComponent &mov);
inline void gotoDest(const Entity &e, const Coordinates &co, Drafted &tag, CombatTemplate<Drafted> &combat, MovementComponent &mov);
inline void gotoTarget(const Entity &e, const Coordinates &co, Drafted &tag, CombatTemplate<Drafted> &combat, MovementComponent &mov);
inline void attackTarget(const Entity &e, const Coordinates &co, Drafted &tag, CombatTemplate<Drafted> &combat, MovementComponent &mov);


void DraftedSystem::update(const double duration)
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
			attackTarget(e, co, tag, combat, mov);
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
#include "ECS\Systems\ai\WorkTemplate.h"
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
}

inline void attackTarget(const Entity & e, const Coordinates & co, Drafted & tag, CombatTemplate<Drafted>& combat, MovementComponent & mov)
{
}
