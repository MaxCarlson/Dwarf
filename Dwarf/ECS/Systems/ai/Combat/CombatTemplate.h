#pragma once
#include "ECS\Components\MovementComponent.h"
#include "Coordinates.h"
#include "Map\Tile.h"
#include "ECS\Messages\entity_moved_message.h"
#include "ECS\Systems\helpers\PathFinding.h"
#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\Fighting\MeleeWeapon.h"
#include "Globals\GlobalWorld.h"
#include "ECS\Messages\damage_entity_message.h"

template<typename Tag>
class CombatTemplate
{
public:

	template<typename CANCEL, typename SUCCESS>
	void getCloseToMovingThing(const Entity &e, MovementComponent &mov, const Coordinates &co, size_t followId, double distance, const CANCEL &cancel, const SUCCESS &success)
	{
		if (mov.progress)
			return;

		const auto& fid = world.getEntity(followId);

		if (!fid.isValid() || !fid.hasComponent<PositionComponent>())
		{
			cancel();
			return;
		}

		const auto& fCo = fid.getComponent<PositionComponent>().co;

		// If we're close enough to the entity we're following
		// stop moving and do whatever is specified in SUCCESS
		if (get_3D_distance(co, fCo) < distance)
		{
			mov.path.clear();
			success();
			return;
		}

		// If the Entity we're trying to get close to is not at where out path ends
		if (mov.path.empty() || mov.path.front() != fCo)
		{
			auto path = findPath(co, fCo);

			if (!path->failed)
			{
				mov.path = path->path;
			}
			else
			{
				cancel();
				return;
			}
		}
	}

	bool isTargetCloseEnough(const double maxDistance, const Coordinates &co, const Coordinates &co2) // TODO: Add in max distance finding based on CombatBase - RangedWeapon vs. MeleeWeapon
	{
		return (get_3D_distance(co, co2) < maxDistance);
	}

	template<typename TOO_FAR>
	void attackEntity(const double &duration, const Entity& e, CombatBase &base, const Entity &target, const TOO_FAR &tooFar) // TODO: Add in a BeingAttacked Component which will alert entities that they have to react. And have a reasponce system
	{
		// Add time in seconds to attack
		base.timeIntoAttack += duration / 1000.0;

		auto& co = e.getComponent<PositionComponent>().co;
		auto& targetCo = target.getComponent<PositionComponent>().co;

		if (!isTargetCloseEnough(base.maxDistance, co, targetCo))
		{
			tooFar();
			return;
		}

		// Finally able to attack!
		if (base.timeIntoAttack > base.attackSpeed)
		{
			if (base.weaponType == CombatBase::MELEE)
			{
				auto* wep = getMeleeWeapon(e);

				// Entity hasn't moved or it's still close enough
				if (wep)
				{
					int dmg = rollForMeleeDmg(e, base, target);

					world.emit(damage_entity_message { dmg, e.getId().index, target.getId().index, DamageTypes::MELEE });
				}
				else if (wep == nullptr)
					base.weaponType = CombatBase::NO_WEAPON;
			}

			else if (base.weaponType == CombatBase::NO_WEAPON)
			{
				int dmg = rollForMeleeDmg(e, base, target);

				world.emit(damage_entity_message { dmg, e.getId().index, target.getId().index, DamageTypes::RANGED });
			}

			else if (base.weaponType == CombatBase::RANGED)
			{

			}

			base.timeIntoAttack = 0.0;
		}
	}

	void cancelCombat(Entity e)
	{
		e.removeComponent<Tag>();
		e.activate();
	}
};