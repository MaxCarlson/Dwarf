#pragma once
#include "../Component.h"
#include "../../Coordinates.h"

// Holds info about the job to be done
// Used in jobs system for assigning jobs
class Job
{
public:
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
	Job(Coordinates co, int exp, int baseSkill, double duration, Job::Jobs jobType, Coordinates coObj = EMPTY_COORDINATES) 
		: co(co), experience(exp), baseSkillReq(baseSkill),  baseDuration(duration), jobType(jobType), coObj(coObj) {}

	inline void reset()
	{
		jobType = NONE;
		//co = EMPTY_COORDINATES;
		//experience = 0;
		//baseSkillReq = 0;
		//baseDuration = 0;
		// None of these should be reset
		// as only jobType should be used to check if this
		// Entity has a job
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

	// Number of times this job has been
	// Attempted by different Entities
	// at a threshold this job will be discarded
	// with a message (eventually)
	int numberOfAttempts = 0;

	// Coordinates of object this job works on
	// Used for jobs like mining so we know where we mined
	// For jobs that don't require it, it'll be set to EMPTY_COORDINATES
	Coordinates coObj;
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

	// This is used by various systems to do extra things
	// on job completion, other than rewarding xp, gaining levels
	bool jobDone = false;

	// Amount of progress done on job
	// This is in seconds and is matched with
	// base duration to determine if the job is complete
	double progress = 0.0;
};

static const std::vector<Job::Jobs> listOfAllJobsByIndex = 
{
	Job::NONE,
	Job::MINER,
	Job::MASON,
	Job::WOODCUTTER,
	Job::CARPENTER
};

static const std::vector<std::string> listOfAllJobsByString =
{
	"None",
	"Miner",
	"Mason",
	"Woodcutter",
	"Carpenter"
};