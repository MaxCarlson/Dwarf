#pragma once
#include "../../Systems.h"

class LaborStatsComponent;
class JobComponent;
class JobsSystem;
class Mining_JC;

class MiningAiSystem : public System<Requires<LaborStatsComponent, JobComponent >>
{
public:
	MiningAiSystem() = default;
	MiningAiSystem(JobsSystem * jobsSystem);

	void update();

	int currentJobs;

private:
	JobsSystem * jobsSystem;

	void createJobs();

	void findPick();
	//void dropPick();
	void setDesination();
	//void tileMined(); // Should this be handled here or in jobs ?
};

