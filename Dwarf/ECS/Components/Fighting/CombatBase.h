#pragma once
#include "ECS\Component.h"

// Entities with this component can attack other entities
struct CombatBase : public Component
{
	enum OnEnemyContact
	{
		FLEE,
		DEFEND,
		ATTACK,
	};

	enum WeaponType
	{
		NO_WEAPON,
		MELEE,
		RANGED
	};

	// Defines how this Entity
	// interact with enimies
	int onEnemyContact = FLEE;

	int weaponType = NO_WEAPON;

	// How much time they've been attacking
	// without their attack yet triggering
	double timeIntoAttack = 0.0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(onEnemyContact, weaponType, timeIntoAttack);
	}
};

static const std::vector<std::string> CombatDecisionNames = { "Flee", "Defend", "Attack" };

CEREAL_REGISTER_TYPE(CombatBase);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, CombatBase);