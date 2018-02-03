#pragma once
#include "ECS\Systems.h"

class DesignBuilding : public System<Requires<>>
{
public:
	DesignBuilding();
	~DesignBuilding();
};

