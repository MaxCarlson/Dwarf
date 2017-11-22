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

	Job() = default;
	Job(Coordinates co, int exp, int baseSkill, double duration, Job::Jobs jobType) 
		: co(co), experience(exp), baseSkillReq(baseSkill),  baseDuration(duration), jobType(jobType) {}

	inline void reset()
	{
		// None of these should be reset
		// as only jobType should be used to check if this
		// Entity has a job
		//co = EMPTY_COORDINATES;
		//experience = 0;
		//baseSkillReq = 0;
		//baseDuration = 0;
		jobType = NONE;
	}

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
	// currentJob.jobType is the only thing checked to see
	// if Entity currently has a job. All other stats are not reset
	// after job has finished
	Job currentJob;

	// Amount of progress done on job
	// This is in seconds and is matched with
	// base duration to determine if the job is complete
	double progress = 0.0;
};