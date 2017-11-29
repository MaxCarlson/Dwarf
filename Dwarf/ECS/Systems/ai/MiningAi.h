#pragma once

#include "../../Systems.h"

class MiningTag;

class MiningAi : public System<Requires<MiningTag>>
{
public:
	MiningAi();
	
	void update(double deltaT);

private:
	void updateMiner(Entity e);
};

