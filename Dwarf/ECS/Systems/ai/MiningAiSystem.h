#pragma once

#include "../../Systems.h"

class LaborStatsComponent;
class JobComponent;

class MiningAiSystem : public System<Requires<LaborStatsComponent, JobComponent>>
{
public:
	MiningAiSystem() = default;

	void update();

private:
	void findPick();
	void setDesination();
	//void tileMined(); // Should this be handled here or in jobs ?
};

