#include "WorkOrderHelper.h"
#include "../Designations.h"
#include "../Raws/ReadReactions.h"
#include "../ECS/Components/Tags/WorkOrderTag.h"
#include "ItemHelper.h"
#include <unordered_set>

std::unique_ptr<WorkOrderHelper> workOrderHelper;

// Indexed by Entity id.index
std::unordered_set<std::size_t> claimed_workshops;

std::unique_ptr<work_order_reaction> WorkOrderHelper::find_work_order_reaction(const WorkOrderTag & tag)
{
	std::unique_ptr<work_order_reaction> wo_reaction;

	if (designations->workOrders.empty())
		return wo_reaction;

	for (auto & des : designations->workOrders)
	{
		auto react = getReaction(des.second);

		// Find a building that can proccess reaction
		bool availible = false;
		std::size_t workshop_id = 0;

		for (const auto& e : getEntities())
		{
			const auto& b = e.getComponent<Building>();

			// Building is correct and is complete
			if (b.tag == react->workshop && b.complete)
			{
				auto isBusy = claimed_workshops.find(e.getId().index);

				// Workshop is not already claimed
				if (isBusy == claimed_workshops.end())
				{
					workshop_id = e.getId().index;
					availible = true;
					break;
				}

			}
		}

		// Eventually test to make sure Entities can actually perform job

		// If availible claim components and set wo_reaction
		if (availible)
		{
			std::vector<std::pair<std::size_t, bool>> components;
			for (auto & in : react->inputs)
			{
				const int num_avail = itemHelper.num_reaction_input_items(in);

				// Not enough components availible
				// Items that are later claimed will be unclaimed
				// momentarily
				if (num_avail < in.quantity)
					availible = false;
				
				// Claim the item
				else
				{
					std::size_t item = itemHelper.claim_item_by_reaction_inp(in);
					components.push_back(std::make_pair(item, false));
				}
					
			}

			// Set work_order_reaction
			// Reduce number of queued work order jobs
			if (availible)
			{
				wo_reaction = std::make_unique<work_order_reaction>( workshop_id, react->name, components );
				claimed_workshops.insert(workshop_id);
				--des.first;
				updateWorkOrders();
			}

			// Unclaim components
			else
			{
				for (auto & comp : components)
					itemHelper.unclaim_item_by_id(comp.first);
			}
		}	
	}

	return wo_reaction;
}

void WorkOrderHelper::unclaim_workshop(const std::size_t id)
{
	claimed_workshops.erase(id);
}

void WorkOrderHelper::updateWorkOrders()
{
	designations->workOrders.erase(
		std::remove_if(designations->workOrders.begin(), designations->workOrders.end(),
			[](auto num) { return num.first < 1; }
		),
		designations->workOrders.end()
	);
}
