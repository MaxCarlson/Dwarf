#pragma once
#include "ECS\Systems.h"

class DesignMining : public System<Requires<>>
{
public:
	DesignMining();
	~DesignMining();
};

