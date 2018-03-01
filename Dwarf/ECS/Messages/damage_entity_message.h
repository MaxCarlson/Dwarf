#pragma once
#include "ECS\Systems.h"

enum class DamageTypes
{
	MELEE,
	RANGED,
	//EXPLOSIVE 
	// TODO: More Types
};

struct damage_entity_message : public base_message_t
{
	damage_entity_message() = default;
	damage_entity_message(int dmg, size_t attacker, size_t target, DamageTypes type) 
		: dmg(dmg), attacker(attacker), target(target), type(type) {}

	int dmg = 0;

	size_t attacker = 0;
	size_t target = 0;

	DamageTypes type;
};