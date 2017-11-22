#pragma once
#include "../Systems.h"

#include "../Components/JobComponent.h"
#include "../Components/LaborStatsComponent.h"
#include <queue>

class JobsSystem : public System<Requires<JobComponent, MovementComponent, PositionComponent, LaborStatsComponent>>
{
public:

	// External interface to start the job system,
	// updates in time steps
	void update(double tStep);

	// External interface to add a job
	// to the job queue, 
	void addJob(Job job);

	// Assign a job to the Entity
	// This will need to be fleshed out in the 
	// future with checks for min lvl req, etc
	bool assignJob(Entity e);

	// Progress in working the current or new job
	void workJob(Entity e, double tStep);

	// Handles the switch to specific job completion
	// actions
	void onJobComplete(Entity & e, Job::Jobs job);

private:

	// List of jobs not currently being worked.
	// Probably make this a vector of priority queues
	// vector by job type, priority queue by user/internal priority.
	std::queue<Job>  notStarted;
};
