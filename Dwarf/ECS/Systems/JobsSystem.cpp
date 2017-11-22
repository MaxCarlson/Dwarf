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
		auto& co = e.getComponent<PositionComponent>().co;

		// If the Entity is at job site
		// start or coninue working
		if (co == job.currentJob.co)
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
	mov.destination = job.currentJob.co;


	// Return value should be used to designate 
	// job was accepted once we have different types of
	// job abilities
	return true;
}

void JobsSystem::workJob(Entity e, double tStep)
{
	// Base time in seconds to progress job progress by
	//
	double tSeconds = tStep / 1000;

	auto& job = e.getComponent<JobComponent>();
	auto& stats = e.getComponent<LaborStatsComponent>();

	
}
