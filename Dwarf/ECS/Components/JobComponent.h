#pragma once
#include "../Component.h"
#include "../../Coordinates.h"

// Holds info about the job to be done
// Used in jobs system for assigning jobs
struct Job
{
	// Used not only for Id'ing job types
	// but also in indexing assciated Job skill in LaborStatsComponent ~!~!~ This should be moved somewhere else? Perhaps even read in serially?
	enum Jobs
	{
		NONE,
		MINER,
		MASON,
		WOODCUTTER,
		CARPENTER
	};
	static const int MAX_LABORS = 5;

	Job(Coordinates co, int exp, int bSkill, double duration, Job::Jobs jobType) 
		: co(co), experience(exp), baseSkillReq(bSkill),  baseDuration(duration), jobType(jobType) {}

	// Job location
	Coordinates co;

	// Base experience gained
	// when finishing job
	int experience;

	// Minimum skill required to do this job,
	int baseSkillReq;

	// How long does the job take
	// with skill of 1? In seconds
	double baseDuration;

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