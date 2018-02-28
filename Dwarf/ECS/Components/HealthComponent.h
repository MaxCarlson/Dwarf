#pragma once

#include "../Component.h"
#include "Raws\Defs\BodyPart.h"

// Main health component, probably going 
// to be used for filtering in systems
// as well as cumulative health
// as body parts will have indiviual health stats
class HealthComponent : public Component
{
public:
	HealthComponent() = default;
	HealthComponent(int health, int maxHealth, int recoverRate, const std::vector<BodyPart>& bodyParts)
		: health(health), maxHealth(maxHealth), recoverRate(recoverRate), bodyParts(bodyParts) {};

	int health;
	int maxHealth;
	int recoverRate;

	std::vector<BodyPart> bodyParts;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(health, maxHealth, recoverRate, bodyParts);
	}
};

CEREAL_REGISTER_TYPE(HealthComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, HealthComponent);