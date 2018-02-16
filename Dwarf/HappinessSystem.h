#pragma once
#include "ECS\Systems.h"
#include "ECS\Components\Sentients\Needs.h"
#include "ECS\Components\Sentients\Happiness.h"

class HappinessSystem : public System<Requires<Happiness, Needs>>
{
public:
	void init();
	void update(const double duration);
};

