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
		auto& mov = e.getComponent<MovementComponent>();

		// If the Entity has a job and 
		// is at job site start or coninue working
		if (job.currentJob.jobType && co == job.currentJob.co)
			workJob(e, tStep);

		// If this Entity failed to reach (or eventually do?) -- Just make sure an Entity can never start or get assigned a job it can't do 
		// this job, try and give it to another Entity
		else if (job.currentJob.jobType && mov.cannotFindPath)
		{
			retryJob(e);
			continue;
		}
			
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

void JobsSystem::retryJob(Entity e)
{
	auto& job = e.getComponent<JobComponent>();
	e.getComponent<MovementComponent>().cannotFindPath = false;

	// If job has been attempted less than 10 times
	// Re add it to the job storage and increment attempts
	if (job.currentJob.numberOfAttempts < 10)
	{
		++job.currentJob.numberOfAttempts;

		notStarted.push(job.currentJob);
	}

	// Reset Entities job to inactive
	job.currentJob.reset();
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
	double workSeconds = JobSpeedMultiplier[stats.laborLevel[jobType]] * tSeconds;

	job.progress += workSeconds;

	// If job has progressed past the duration set
	// by job creator the job is finished
	if (job.progress >= job.currentJob.baseDuration)
	{
		// Set the current job to NONE
		job.currentJob.reset();
		
		// Add experience to the Entity for completing job
		stats.skillPoints[jobType] += job.currentJob.experience;

		// If Entity has enough skill points in a labor, and they're not at
		// max labor level, increase their skill level by one
		if (laborSkillIncrease(stats.skillPoints[jobType], stats.laborLevel[jobType]))
			stats.laborLevel[jobType] += 1;

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
