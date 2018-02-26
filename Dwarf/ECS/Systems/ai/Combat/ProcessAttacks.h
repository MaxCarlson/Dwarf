#pragma once
#include "ECS\Systems.h"

class ProcessAttacks : public System<Requires<>>
{
public:
	void init();
	void update(const double duration);
};