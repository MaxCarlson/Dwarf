#pragma once
#include "ECS\Systems.h"

class DesignWoodcutting : public System<Requires<>>
{
public:
	void init();
	void update(const double duration);
};

