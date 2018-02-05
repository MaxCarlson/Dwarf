#pragma once
#include "ECS\Systems.h"

class DesignArchitecture : public System<Requires<>>
{
public:
	void init();
	void update(const double duration);
};

