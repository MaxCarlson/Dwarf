#pragma once
#include "ECS\Systems.h"
#include "ECS\Components\Tags\EatFoodTag.h"
#include "ECS\Components\Sentients\Needs.h"

class EatFoodSystem : public System<Requires<EatFoodTag, Needs>>
{
public:
	void init();
	void update(const double duration);
private:
};