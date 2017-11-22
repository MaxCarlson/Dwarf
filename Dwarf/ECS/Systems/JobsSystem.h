#pragma once
#include "../Systems.h"

#include "../Components/JobComponent.h"
#include <queue>

class JobsSystem : public System<Requires<JobComponent>>
{
public:

	void update(double tStep);


	void addJob(Job job);
	bool assignJob(Entity e);
	void workJob(Entity e, double tStep);

private:
	std::queue<Job>  notStarted;
};