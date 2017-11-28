#include "MiningAiSystem.h"
#include "../JobsSystem.h"
#include "../Designations.h"
#include "../MiningSystem.h"
#include "../TileFactory.h"
#include "../Designations.h"



MiningAiSystem::MiningAiSystem(MiningSystem * miningSystem, TileFactory * tileFactory) : miningSystem(miningSystem), tileFactory(tileFactory)
{
}

void MiningAiSystem::update()
{
	// If there's nothing designated, there are no jobs to do!
	if (designations->mining.empty())
		return;

	const auto& entities = getEntities();

	for (auto& e : entities)
	{
		auto& job = e.getComponent<JobComponent>();

		// If Entity is not currently a miner, or if
		// their job is not done yet, skip them
		if (!job.currentJob.jobType == Job::MINER || !job.jobDone)
			continue;

		

		mineTile(e);
	}
	
}

void MiningAiSystem::mineTile(const Entity e)
{
	auto& job = e.getComponent<JobComponent>();

	// Delete job from current active jobs in MiningSystem
	miningSystem->updateCurrentJobs(job.currentJob);

	tileFactory->createRockFloor(job.currentJob.coObj);

	designations->mining.erase(job.currentJob.coObj);

	job.jobDone = false;
	job.currentJob.reset();

	// Tell mining System it needs to update
	// and possibly issue more jobs ~!~~!~~! Make this into a lambda message with boost bind?
	miningSystem->updateMap();
}

