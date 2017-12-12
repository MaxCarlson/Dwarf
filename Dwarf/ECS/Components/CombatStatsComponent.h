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

CEREAL_REGISTER_TYPE(CombatStatComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, CombatStatComponent);