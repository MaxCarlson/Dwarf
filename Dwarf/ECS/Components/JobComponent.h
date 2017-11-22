#pragma once
#include "../Component.h"
#include "../../Coordinates.h"

// Holds info about the job to be done
// Used in jobs system for assigning jobs
struct Job
{
	// Job location
	Coordinates co;

	enum Jobs
	{
		MINE = 1,
		CHOP_WOOD,
		MASONRY
	};

	// Job type
	Jobs jobType;
};

// Going to be used for filtering capable
// Entities, as well as storing current job info
class JobComponent : public Component
{
public:
	JobComponent() = default;

	// currentJob.jobType = 0 for no Job
	Job currentJob;

	// Indicates if and when the Entity 
	// should start working on job
	bool started;

	// Amount of progress done on job
	double progress = 0.0;
};