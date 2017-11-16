#pragma once
#include "../Component.h"
#include "../../Coordinates.h"

// Going to be used for filtering capable
// Entities, as well as storing current job info
class JobComponent : public Component
{
public:
	JobComponent() = default;

	// 0 = no current job
	int currentJob;

	enum Jobs
	{
		MINE = 1,
		CHOP_WOOD
	};
};

// Holds info about the job to be done
// Used in jobs system for assigning jobs
struct Job
{
	Coordinates co;
	JobComponent::Jobs jobType;

};