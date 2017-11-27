#pragma once
#include "../../Systems.h"

class LaborStatsComponent;
class JobComponent;
class JobsSystem;

class MiningAiSystem : public System<Requires<LaborStatsComponent, JobComponent>>
{
public:
	MiningAiSystem() = default;
	MiningAiSystem(JobsSystem * jobsSystem);

	void update();

private:
	JobsSystem * jobsSystem;

	void findPick();
	void dropPick();
	void setDesination();
	//void tileMined(); // Should this be handled here or in jobs ?
};

