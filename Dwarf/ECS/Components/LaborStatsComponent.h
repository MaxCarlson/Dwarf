#pragma once

#include "../Component.h"
#include "JobComponent.h"

static const int MAX_SKILL_LVL = 25;

// Values just placeholders
static const int skillLevelReq[MAX_SKILL_LVL] = { 0, 0, 100, 300, 500, 1100, 1800, 2600, 3500, 4500, 5600, 6800, 8100, 9500, 11000, 12600, 14300, 16100, 18000, 20000, 22100, 24300, 26600, 29000 };

// Increase in speed for jobs being done by Entities
// with index skill level
static const double JobSpeedMultiplier[MAX_SKILL_LVL] = { 1, 1, 1.05, 1.10, 1.15, 1.21, 1.27, 1.33, 1.40, 1.47, 1.54, 1.62, 1.70, 1.78, 1.86, 1.96, 2.06, 2.18, 2.30, 2.50 };

// Need job quality multiplier based on skill level, with random range of qualities

// Holds data on all stats creatures have
// Possibly split this in two, one for dwarves
// on for enemys?
class LaborStatsComponent : public Component
{
public:
	LaborStatsComponent() = default;

	// Array of LaborStats indexed by Job::Jobs,
	// for skill level associated with jobType
	// Stats corrispond to skillLevel points from skillLevelReq
	int laborStats[Job::MAX_LABORS] = { 1 };

	// Total points accumulated for Labor Skills
	// Used to determine when to bump up laborStats by one level
	// with skillLevelReq[laborStats[JobType]+1]
	int skillLevel[Job::MAX_LABORS] = { 0 };
};