#pragma once
#include "ECS\Systems.h"
#include "ECS\Components\Fighting\Drafted.h"

class DraftedSystem : public System<Requires<Drafted>>
{
public:
	void init();
	void update(const double duration);
};