#include "MeleeWeapon.h"
#include "Raws\Defs\ItemDefs.h"
#include "Raws\ItemRead.h"
#include "ECS\Components\Quality.h"
#include "Raws\Materials.h"
#include "ECS\Components\Item.h"
#include "Raws\Defs\MaterialDef.h"

void calcualteMeleeWeaponStats(MeleeWeapon & melee, Item & item, size_t material, int quality)
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

	auto* itemDef = getItemDef(item.tag);

	if (itemDef != nullptr)
	{
		baseDmg = itemDef->baseDmg;
		baseSpeed = itemDef->baseSpeed;
		twoHanded = itemDef->twoHanded;
	}

	melee.dmg = qualityMeleeMultiplier(quality) * baseDmg * matDmg;
	melee.speed = baseSpeed * matSpeed;
	melee.twoHand = twoHanded;
}
