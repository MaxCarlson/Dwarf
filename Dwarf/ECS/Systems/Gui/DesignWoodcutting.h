#pragma once
#include "ECS\Systems.h"

class DesignWoodcutting : public System<Requires<>>
{
public:
	DesignWoodcutting();
	~DesignWoodcutting();
};

