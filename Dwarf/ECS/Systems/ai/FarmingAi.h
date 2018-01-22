#pragma once
#include "../../Systems.h"
#include "../../Components/Tags/PlantingTag.h"

class FarmingAi : public System<Requires<PlantingTag>>
{
public:
	FarmingAi();
	~FarmingAi();
};

