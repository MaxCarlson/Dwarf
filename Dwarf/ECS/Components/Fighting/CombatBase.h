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

	// This is a combined speed using entity stats
	// as well as weapon speed to determine it.
	// Any time a weapon is dropped or picked up,
	// or and entity levels a speed skill 
	// this will be updated
	double attackSpeed = 1.0; 

	// This is also calculated any time 
	// and entity picks up / drop a weapon
	// or levels up
	double baseDmg = 1.0;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(onEnemyContact, weaponType, timeIntoAttack, attackSpeed, baseDmg);
	}
};

void calculateCombatBase(const Entity &e, const size_t wep); // TODO: Add in armour values

static const std::vector<std::string> CombatDecisionNames = { "Flee", "Defend", "Attack" };

CEREAL_REGISTER_TYPE(CombatBase);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, CombatBase);