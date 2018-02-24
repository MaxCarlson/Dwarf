#pragma once
#include "ECS\Component.h"

struct MeleeWeapon : public Component
{

	// This is calculated on creation by item quality,
	// material dmg multiplier
	// Combined with other stats to get true damage
	double dmg = 1.0; // TODO: Change damage types for different weapons!

	// Calculated on creation by material weight
	// and base weapon speed
	// How often the weapon attacks in seconds,
	// combined with other stats to get true speed
	double speed = 1.0;

	bool twoHand = false;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(dmg, speed, twoHand);
	}
};

void createMeleeWeapon(Entity &e, const std::string &itemTag, size_t material, int quality);

CEREAL_REGISTER_TYPE(MeleeWeapon);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, MeleeWeapon);