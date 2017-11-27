#include "MiningAiSystem.h"
#include "../JobsSystem.h"
#include "../Designations.h"



MiningAiSystem::MiningAiSystem(JobsSystem * jobsSystem) : jobsSystem(jobsSystem)
{
}

void MiningAiSystem::update()
{
	// If there's nothing designated, there are no jobs to do!
	if (designations->mining.empty())
		return;

	
}

void MiningAiSystem::setDesination()
{

}
