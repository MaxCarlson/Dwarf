#pragma once
#include "ECS\Systems.h"
#include "ECS\Components\Fighting\Drafted.h"
#include "ECS\Components\Fighting\CombatBase.h"

class DraftedSystem : public System<Requires<Drafted, CombatBase>>
{
public:
	void init();
	void update(const double duration);
};