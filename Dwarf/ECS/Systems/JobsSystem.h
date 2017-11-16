#pragma once
#include "../Systems.h"

#include "../Components/JobComponent.h"
#include <queue>

class JobsSystem : public System<Requires<JobComponent>>
{
public:

	void update()
	{

	}

	void addJob(Job job)
	{
		notStartedJobs.push(job);
	}

private:
	std::queue<Job> notStartedJobs;
	std::queue<Job> unfinishedJobs;
};