#pragma once

#include "../Component.h"


enum CombatStats
{
	NONE,
	SWORD,
	AXE,
	MACE,
	SPEAR,
	DAGGER,
	CROSSBOW,
	BOW
};

class CombatStatComponent : public Component
{
	CombatStatComponent() = default;

	template<class Archive>
	void serialize(Archive& archive)
	{

	}
};