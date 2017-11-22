#include "JobsSystem.h"
#include "../Components/PositionComponent.h"


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

	// This needs to be redone to take into accound
	// individual Dwarf stats, prefrences, priorities, etc.
	job.currentJob = notStarted.front();
	notStarted.pop();
}

void JobsSystem::workJob(Entity e, double tStep)
{

}
