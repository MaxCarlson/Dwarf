#include "MiningAiSystem.h"
#include "../JobsSystem.h"
#include "../Designations.h"
#include "../MiningSystem.h"



MiningAiSystem::MiningAiSystem(JobsSystem * jobsSystem) : jobsSystem(jobsSystem)
{
}

void MiningAiSystem::update()
{
	// If there's nothing designated, there are no jobs to do!
	if (designations->mining.empty())
		return;

	if (currentJobs == miningMap.f)
		return;

	const auto& entities = getEntities();

	for (auto& e : entities)
	{

	}
	
}

void MiningAiSystem::createJobs()
{
	for (int i = 0; i < designations->mining.count(0); ++i)
	{
		Job newJob()
		jobsSystem->addJob()
	}
	
}

void MiningAiSystem::findPick()
{
}

void MiningAiSystem::setDesination()
{

}
