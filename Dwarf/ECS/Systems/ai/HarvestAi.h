#pragma once
#include "../../Systems.h"
#include "../../Components/Tags/HarvestTag.h"

class HarvestAi : public System<Requires<HarvestTag>>
{
public:
	
	void init();

	void update(const double duration);

private:
	void doHarvest(const Entity& e, const double& duration);
};

