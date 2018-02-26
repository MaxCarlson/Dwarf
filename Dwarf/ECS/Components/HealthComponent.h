#pragma once

#include "../Component.h"

// Main health component, probably going 
// to be used for filtering in systems
// as well as cumulative health
// as body parts will have indiviual health stats
class HealthComponent : public Component
{
public:
	HealthComponent() = default;
	HealthComponent(int health, int maxHealth, int recoverRate) 
		: health(health), maxHealth(maxHealth), recoverRate(recoverRate) {};

	int health;
	int maxHealth;
	int recoverRate;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(health, maxHealth, recoverRate);
	}
};

CEREAL_REGISTER_TYPE(HealthComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, HealthComponent);