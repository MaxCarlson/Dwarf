#pragma once
#include "ECS\Systems.h"
class DesignHarvest : public System<Requires<>>
{
public:
	DesignHarvest();
	~DesignHarvest();
};

