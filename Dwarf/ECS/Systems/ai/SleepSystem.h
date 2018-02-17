#pragma once
#include "ECS\Systems.h"
#include "ECS\Components\Tags\SleepTag.h"

class SleepSystem : public System<Requires<SleepTag>>
{
public:
	void init();
	void update(const double duration);
};

