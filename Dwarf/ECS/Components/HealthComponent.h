#pragma once

#include "../Component.h"

class HealthComponent : public Component
{
public:
	//HealthComponent() = default;
	HealthComponent(int health, int maxHealth, int recoverRate) 
		: health(health), maxHealth(maxHealth), recoverRate(recoverRate) {};

	int health;
	int maxHealth;
	int recoverRate;
};