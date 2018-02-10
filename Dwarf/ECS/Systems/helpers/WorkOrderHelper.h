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

	// Updated very infrequently, only to check for new reactions
	// that may be possible
	void update(const double duration);

	int claimedWorkshops();

	int scanForBestWorkOrder(const Entity& e);
	std::unique_ptr<work_order_reaction> find_work_order_reaction(const WorkOrderTag & tag);

	void unclaim_workshop(const std::size_t id);

private:
	// Indexed by Entity id.index
	std::unordered_set<std::size_t> claimed_workshops;

	void updateWorkOrders(std::vector<std::pair<int, std::string>>& des);
};

extern std::unique_ptr<WorkOrderHelper> workOrderHelper;


