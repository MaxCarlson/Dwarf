#pragma once

#include "../Component.h"
#include "JobComponent.h"
#include <vector>

static const int MAX_LABOR_LVL = 25;

// Values just placeholders
static const int laborLevelReq[MAX_LABOR_LVL] = { 0, 0, 100, 300, 500, 1100, 1800, 2600, 3500, 4500, 5600, 6800, 8100, 9500, 11000, 12600, 14300, 16100, 18000, 20000, 22100, 24300, 26600, 29000 };

// Increase in speed for jobs being done by Entities
// with index skill level
static const double JobSpeedMultiplier[MAX_LABOR_LVL] = { 1, 1, 1.05, 1.10, 1.15, 1.21, 1.27, 1.33, 1.40, 1.47, 1.54, 1.62, 1.70, 1.78, 1.86, 1.96, 2.06, 2.18, 2.30, 2.50 };

// Need job quality multiplier based on skill level, with random range of qualities

// Holds data on all stats creatures have
// Possibly split this in two, one for dwarves
// on for enemys?
class LaborStatsComponent : public Component
{
public:
	LaborStatsComponent() = default;
	LaborStatsComponent(std::vector<int> laborLevel, std::vector<int> skillPoints) : laborLevel(laborLevel), skillPoints(skillPoints) {};

	// Array of LaborStats indexed by Job::Jobs,
	// for skill level associated with jobType
	// Stats corrispond to skillLevel points from skillLevelReq
	std::vector<int> laborLevel;

	// Total points accumulated for Labor Skills
	// Used to determine when to bump up laborStats by one level
	// with skillLevelReq[laborStats[JobType]+1]
	std::vector<int> skillPoints;
};

// Determine exact labor skill level by looping through
// labor level requirements
inline int determineLaborSkillLvl(int skillPoints)
{
	for (int i = 0; i < MAX_LABOR_LVL; ++i)
	{
		if (skillPoints < laborLevelReq[i])
			return i - 1;
	}
}

// Used for determining if an Entities labor skill level should be increased by one
inline bool laborSkillIncrease(int skillPoints, int laborSkillLvl)
{
	if (laborSkillLvl >= MAX_LABOR_LVL)
		return false;

	if (skillPoints > laborLevelReq[laborSkillLvl])
		return true;

	return false;
}