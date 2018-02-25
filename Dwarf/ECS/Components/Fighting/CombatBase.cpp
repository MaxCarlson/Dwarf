#include "stdafx.h"
#include "CombatBase.h"
#include "Raws\ItemRead.h"
#include "Raws\Defs\ItemDefs.h"
#include "ECS\Components\Fighting\MeleeWeapon.h"
#include "ECS\Components\Sentients\Stats.h"
#include "Globals\GlobalWorld.h"

static const std::string strength = "strength";
static const std::string agility = "agility";
static const std::string meleeStatsStr = "melee";
static const std::string rangedStatsStr = "ranged";
constexpr double MAX_MELEE_DIST = 1.45;

void calculateCombatBase(const Entity & e, const size_t wep)
{
	if (!e.hasComponent<CombatBase>() || !e.hasComponent<Stats>())
		return;

	auto& base = e.getComponent<CombatBase>();
	auto& stats = e.getComponent<Stats>();

	auto wepEnt = world.getEntity(wep);

	auto strLvl = static_cast<double>(getSkillLvl(stats, strength) + 1);
	auto agiLvl = static_cast<double>(getSkillLvl(stats,  agility) + 1);

	// Weapon
	if (wepEnt.isValid())
	{
		// Melee item
		if (wepEnt.hasComponent<MeleeWeapon>())
		{
			auto& wep = wepEnt.getComponent<MeleeWeapon>();
			auto meleeLvl = static_cast<double>(getSkillLvl(stats, meleeStatsStr)) + 1.0;

			base.baseDmg = wep.dmg + wep.dmg * (meleeLvl * 0.1); // 10% increased damage per melee skill level
			base.attackSpeed = wep.speed + wep.speed * (meleeLvl * 0.03); // 3% speed per melee skill level
			base.maxDistance = MAX_MELEE_DIST;

			// TODO: Add damage for strength
			// TODO: Add speed for agility
		}

		// Ranged Item
	}

	// No Weapon
	else
	{
		constexpr double baseDmg = 0.8; // TODO: Make this race dependant?
		constexpr double baseSpeed = 0.9; // TODO: Race  or agility dependant?

		base.baseDmg = baseDmg + baseDmg * (strLvl * 0.06);
		base.attackSpeed = baseSpeed + baseSpeed * (agiLvl * 0.04);
		base.maxDistance = MAX_MELEE_DIST;
	}
}
