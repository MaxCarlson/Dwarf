#include "JobsSystem.h"
#include "../Components/PositionComponent.h"
#include "../Components/MovementComponent.h"


void JobsSystem::update(double tStep)
{
	auto& entities = getEntities();

	// Loop through all entities with JobComponent
	for (auto& e : entities)
	{
		auto& job = e.getComponent<JobComponent>();
		auto& co  = e.getComponent<PositionComponent>().co;

		// If the Entity has a job and 
		// is at job site start or coninue working
		if (job.currentJob.jobType && co == job.currentJob.co)
			workJob(e, tStep);

		// If the Entity has no job,
		// and there are jobs try to assign it one
		if (!job.currentJob.jobType && !notStarted.empty()
			&& assignJob(e))
			continue;
	}
}

void JobsSystem::addJob(Job job)
{
	notStarted.push(job);
}

bool JobsSystem::assignJob(Entity e)
{
	auto& job = e.getComponent<JobComponent>();
	auto& mov = e.getComponent<MovementComponent>();

	// This needs to be redone to take into accound
	// individual Dwarf stats, prefrences, priorities, etc.
	job.currentJob = notStarted.front();
	notStarted.pop();

	// Set Entity destination for job
	// Pathfinding will try and locate this position
	mov.destination = job.currentJob.co;

	// Return value should be used to designate 
	// job was accepted once we have different types of
	// job abilities
	return true;
}

// Need to add quality multiplier to Jobs that can produce
// quality goods!!
void JobsSystem::workJob(Entity e, double tStep)
{
	// Base time in seconds to progress job progress by
	//
	const double tSeconds = tStep / 1000;

	auto& job   = e.getComponent<JobComponent>();
	auto& stats = e.getComponent<LaborStatsComponent>();

	const auto jobType = job.currentJob.jobType;

	// Multiply time step seconds by Entity skill level job speed multiplier
	double workSeconds = JobSpeedMultiplier[stats.laborStats[jobType]] * tSeconds;

	job.progress += workSeconds;

	// If job has progressed past the duration set
	// by job creator the job is finished
	if (job.progress >= job.currentJob.baseDuration)
	{
		// Set the current job to NONE
		job.currentJob.jobType = Job::NONE;
		
		// Add experience to the Entity for completing job
		stats.skillLevel[jobType] += job.currentJob.experience;

		// If the Entities skill stat is less than max
		// and their skill level >= the required number to gain an experience
		// level in this jobType, raise their level
		if (stats.laborStats[jobType] < MAX_SKILL_LVL
			&& stats.skillLevel[jobType] >= skillLevelReq[stats.laborStats[jobType] + 1])
			stats.laborStats[jobType] += 1;

		onJobComplete(e, jobType);
	}
}

void JobsSystem::onJobComplete(Entity & e, Job::Jobs job)
{
	switch (job)
	{
	case Job::MINER:
		mining(e);
		break;

	case Job::MASON:
		break;

	case Job::WOODCUTTER:
		break;

	case Job::CARPENTER:
		break;
	}

	return;
}

void JobsSystem::mining(Entity & e)
{
	// Lookup what stone/Ore/Gem was mined

	// Based on material type and miner skill
	// decided whether to produce stone/Ore/Gem

	// Add an Entity of that type
}
