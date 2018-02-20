#pragma once
#include "ECS\Systems.h"
#include "ECS\Components\Sentients\Needs.h"

class NeedsSystem : public System<Requires<Needs>>
{
public:
	void init();
	void update(const double duration);
};

