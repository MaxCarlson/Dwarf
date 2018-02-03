#pragma once
#include "ECS\Systems.h"

class DesignDevMode : public System<Requires<>>
{
public:
	void init();
	void update(const double duration);
};

