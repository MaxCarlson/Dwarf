#pragma once
#include "ECS\Systems.h"
#include "ECS\Components\Tags\FarmClearTag.h"

class FarmingClearAi : public System<Requires<FarmClearTag>>
{
public:
	void init();
	void update(const double duration);
};

