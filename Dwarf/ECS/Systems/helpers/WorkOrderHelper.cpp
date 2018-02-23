#include "../stdafx.h"
#include "WorkOrderHelper.h"
#include "Designations\WorkOrderDesignation.h"
#include "../Raws/ReadReactions.h"
#include "../ECS/Components/Tags/WorkOrderTag.h"
#include "../ECS/Components/Sentients/AiWorkComponent.h"
#include "ItemHelper.h"


std::unique_ptr<WorkOrderHelper> workOrderHelper;

void WorkOrderHelper::loopThroughQueuedAndNon(std::vector<WorkOrderDesignation> &work, bool queued)
{
	for (auto& des : work)
	{
		// Skip any reaction that has less than one requested
		// job remaining. All queued work orderes that are added to workorders
		// will be cleared for queuedWorkOrders
		if (des.count < 1)
			continue;

		auto react = getReaction(des.tag);

		// Find a building that can proccess reaction
		bool availible = false;

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
					availible = true;
					break;
				}
			}
		}
		// Eventually test to make sure Entities can actually perform job

		// Make sure that enough components are availible,
		// and that the reaction can be completed. Else remove it from the active work orders
		// or if we're searching the queued work orders and it is possible, add it to the active orders
		if (availible)
		{
			// Look through the workOrderDesignation inputs instead of reaction inputs
			// as there may be a specified material
			for (auto & in : des.inputs)
			{
				const int num_avail = itemHelper.num_reaction_input_items(in);

				// Not enough components availible
				if (num_avail < in.quantity)
				{
					availible = false;
					break;
				}
			}

			// If a work order is no longer completable
			// push it back into queued work orders
			if (!availible && !queued)
			{
				workOrders.queued.emplace_back(des);
				des.count = 0;
			}

			// If a queued work order is completable
			// Push work order to completable tasks
			// and mark work order removal from queued reactions
			else if (availible && queued)
			{
				workOrders.active.emplace_back(des);
				des.count = 0;
			}
		}
	}

	// Remove any workorders that can no longer be completed
	// and add any that can now be completed
	if (!queued)
		updateWorkOrders(workOrders.active);

	// Add any work orders that can no longer be completed
	// and remove those that can
	else
		updateWorkOrders(workOrders.queued);
}

void WorkOrderHelper::update(const double duration)
{
	loopThroughQueuedAndNon(workOrders.active, false);
	loopThroughQueuedAndNon(workOrders.queued, true);
}

// Scan for the best workorder for the entity based on
// job preferences!
// Returns the value of highest rated job and the distance the complete that job
// Right now distance does not change. Will have to evaluate later and see if it's worth calculating distance for these
std::pair<int, int> WorkOrderHelper::scanForBestWorkOrder(const AiWorkComponent & prefs)
{
	int best = 0;
	constexpr int dist = 50; 
	for (const auto& wo : workOrders.active) // Only scan through active work orders
	{	
		const auto& skill = getReaction(wo.tag)->skill;
		const auto find = prefs.jobPrefrences.find(skill);

		// Job preferences indexed by skill
		if (find != prefs.jobPrefrences.end() && find->second > best)
		{
			best = find->second;
		}
	}

	return std::make_pair(best, dist);
}

std::unique_ptr<work_order_reaction> WorkOrderHelper::findWorkOrderReaction(const WorkOrderTag & tag)
{
	std::unique_ptr<work_order_reaction> wo_reaction;

	if (workOrders.active.empty())
		return wo_reaction;


	for (auto& des : workOrders.active)
	{
		// Skip any reaction that has less than one requested
		// job remaining. All reactions < 1 will be cleared from designations
		// after this loop
		if (des.count < 1)
			continue;

		auto react = getReaction(des.tag);

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
			for (auto & in : des.inputs)
			{
				std::vector<std::pair<std::size_t, bool>> tmpComponents;
				const bool enoughItems = itemHelper.enoughItemsForWorkOrder(in, tmpComponents);

				// Not enough components availible
				if (!enoughItems)
				{
					availible = false;
					break;
				}

				// Build the list of componenet Id's 
				components.insert(std::end(components), std::begin(tmpComponents), std::end(tmpComponents));
			}

			// Set work_order_reaction
			// Reduce number of queued work order jobs
			if (availible)
			{
				// Claim all the components
				for (const auto& c : components)
					itemHelper.claim_item(getWorld().getEntity(c.first));

				const auto& woCo = getWorld().getEntity(workshop_id).getComponent<PositionComponent>().co;
				wo_reaction = std::make_unique<work_order_reaction>(workshop_id, woCo, react->tag, components, des.material);
				claimed_workshops.insert(workshop_id);
				--des.count;
				break;
			}
		}
	}

	updateWorkOrders(workOrders.active);

	return wo_reaction;
}

int WorkOrderHelper::claimedWorkshops()
{
	return claimed_workshops.size();
}

void WorkOrderHelper::unclaim_workshop(const std::size_t id)
{
	claimed_workshops.erase(id);
}

void WorkOrderHelper::updateWorkOrders(std::vector<WorkOrderDesignation> &des)
{
	des.erase(
		std::remove_if(des.begin(),des.end(),
			[](WorkOrderDesignation w) { return w.count < 1; }
		),
		des.end()
	);
}
/*
std::unique_ptr<work_order_reaction> WorkOrderHelper::find_work_order_reaction(const WorkOrderTag & tag) // TODO: Possibly add a workshop component to workshop buildings for faster processing!
{
std::unique_ptr<work_order_reaction> wo_reaction;

if (workOrders.active.empty())
return wo_reaction;


for (auto& des : workOrders.active)
{
// Skip any reaction that has less than one requested
// job remaining. All reactions < 1 will be cleared from designations
// after this loop
if (des.count < 1)
continue;

auto react = getReaction(des.tag);

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
const auto& woCo = getWorld().getEntity(workshop_id).getComponent<PositionComponent>().co;
wo_reaction = std::make_unique<work_order_reaction>( workshop_id, woCo, react->tag, components );
claimed_workshops.insert(workshop_id);
--des.count;
break;
}

// Unclaim components
else
{
for (auto & comp : components)
itemHelper.unclaim_item_by_id(comp.first);
}
}
}

updateWorkOrders(workOrders.active);

return wo_reaction;
}
*/