#pragma once
#include "ECS\Systems.h"

class DesignArchitecture : public System<Requires<>>
{
public:
	DesignArchitecture();
	~DesignArchitecture();
};

