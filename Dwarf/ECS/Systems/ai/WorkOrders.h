#pragma once
#include "../../Systems.h"
#include "../../Components/Tags/WorkOrderTag.h"

class WorkOrders : public System<Requires<WorkOrderTag>>
{
public:
	void init();
	void update(const double duration);

private:
	void work(Entity e, const double& duration);
};
