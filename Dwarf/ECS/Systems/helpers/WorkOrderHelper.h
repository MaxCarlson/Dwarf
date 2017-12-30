#pragma once
#include <memory>
#include "../../Systems.h"
#include "../../Components/Building.h"

struct work_order_reaction;
struct WorkOrderTag;

class WorkOrderHelper : public System<Requires<Building>>
{
public:
	WorkOrderHelper() = default;

	//void update(); ~ Non updating

	std::unique_ptr<work_order_reaction> find_work_order_reaction(const WorkOrderTag & tag);

	void unclaim_workshop(const std::size_t id);

private:
	void updateWorkOrders();
};


