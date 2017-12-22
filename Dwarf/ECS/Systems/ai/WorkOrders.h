#pragma once
#include "../../Systems.h"
#include "../../Components/Tags/WorkOrderTag.h"

class WorkOrders : public System<Requires<WorkOrderTag>>
{
public:
	void update(double duration);

private:
};
