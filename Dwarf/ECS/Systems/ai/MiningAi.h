#pragma once

#include "../../Systems.h"

#include "../../Components/Tags/MiningTag.h"

class MiningAi : public System<Requires<MiningTag>>
{
public:
	MiningAi() = default;

	void init();
	
	void update(double deltaT);

private:
	void updateMiner(Entity e);
};

