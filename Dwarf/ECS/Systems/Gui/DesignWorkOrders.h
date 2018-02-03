#pragma once
#include "ECS\Systems.h"

class DesignWorkOrders : public System<Requires<>>
{
public:
	DesignWorkOrders();
	~DesignWorkOrders();
};

