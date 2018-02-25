#include "MeleeWeapon.h"
#include "Raws\Defs\ItemDefs.h"
#include "Raws\ItemRead.h"
#include "ECS\Components\Quality.h"
#include "Raws\Materials.h"
#include "Raws\Defs\MaterialDef.h"
#include "ECS\Components\Sentients\Inventory.h"
#include "ECS\Components\Sentients\Stats.h"
#include "ECS\Components\Fighting\CombatBase.h"

void createMeleeWeapon(Entity &e, const std::string &itemTag, size_t material, int quality)
{
	auto* mat = getMaterial(material);

	double matDmg = 1.0;
	double matSpeed = 1.0;

	if (mat != nullptr)
	{
		matDmg = mat->dmgMultiplier;
		matSpeed = mat->speedMultiplier;
	}

	double baseDmg = 1.0;
	double baseSpeed = 1.0;
	bool twoHanded = true;

	auto* itemDef = getItemDef(itemTag);

	if (itemDef != nullptr)
	{
		baseDmg = itemDef->baseDmg;
		baseSpeed = itemDef->baseSpeed;
		twoHanded = itemDef->twoHanded;
	}

	auto& melee = e.addComponent<MeleeWeapon>();

	melee.dmg = qualityMeleeMultiplier(quality) * baseDmg * matDmg;
	melee.speed = baseSpeed * matSpeed; // TODO: Add slight qulity speed bonus?
	melee.twoHand = twoHanded;

	e.activate();
}

MeleeWeapon * getMeleeWeapon(const Entity & e)
{
	if (!e.hasComponent<Inventory>())
		return nullptr;

	size_t wepId = e.getComponent<Inventory>().inventory[SLOT_WEAPON];

	const auto& wepEnt = e.getWorld().getEntity(wepId);

	if(!wepEnt.isValid() || !wepEnt.hasComponent<MeleeWeapon>())
		return nullptr;

	return &wepEnt.getComponent<MeleeWeapon>();
}

double rollForMeleeDmg(const Entity & e, const CombatBase &base, const Entity &target, const MeleeWeapon & wep)
{

	

	return 0.0;
}
