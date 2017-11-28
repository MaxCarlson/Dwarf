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

	const auto& entities = getEntities();

	for (auto& e : entities)
	{

	}
	
}

void MiningAiSystem::createJobs()
{
	
}

void MiningAiSystem::findPick()
{
}

void MiningAiSystem::setDesination()
{

}
