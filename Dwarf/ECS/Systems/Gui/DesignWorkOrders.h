#pragma once
#include "ECS\Systems.h"

class DesignWorkOrders : public System<Requires<>>
{
public:
	void init();
	void update(const double duration);

private:
	void giveOrder();
	void drawJobs();
};

