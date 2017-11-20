#pragma once

#include "../Component.h"

// Stats for Labors
// Probably only going to be useful
// for player controled chars
struct LaborStats
{
	int miner = 0;
	int mason = 0;
	int woodcutter = 0;
	int carpenter = 0;
};

// Stats for combat
struct CombatStats
{
	int sword = 0;
	int axe = 0;
	int mace = 0;
	int spear = 0;
	int dagger = 0;
	int crossbow = 0;
	int bow = 0;
};

// Holds data on all stats creatures have
// Possibly split this in two, one for dwarves
// on for enemys?
class CreatureStatsComponent : public Component
{
public:
	CreatureStatsComponent() = default;
	CreatureStatsComponent(LaborStats  laborStats,  CombatStats combatStats) 
		                   : laborStats(laborStats), combatStats(combatStats) {};

	LaborStats   laborStats;
	CombatStats combatStats;
};