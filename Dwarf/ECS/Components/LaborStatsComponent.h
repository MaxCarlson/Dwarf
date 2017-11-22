#pragma once

#include "../Component.h"
#include "JobComponent.h"


// Holds data on all stats creatures have
// Possibly split this in two, one for dwarves
// on for enemys?
class LaborStatsComponent : public Component
{
public:
	LaborStatsComponent() = default;
	LaborStatsComponent() {};

	// Array of LaborStats indexed by Job::Jobs,
	// for skill level associated with jobType
	int laborStats[Job::MAX_LABORS] = { 1 };

	// How far has this Entity progressed toward next skill level?
	double laborProgress[Job::MAX_LABORS] = { 0 };
};