#pragma once
#include "ECS\Component.h"

struct MeleeWeapon : public Component
{

	// This is calculated on creation by item quality,
	// material dmg multiplier
	double dmg;

	// Calculated on creation by material weight
	// and base weapon speed
	// Speed in seconds
	double speed;

	bool twoHand = false;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(dmg, speed, twoHand);
	}
};

CEREAL_REGISTER_TYPE(MeleeWeapon);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, MeleeWeapon);