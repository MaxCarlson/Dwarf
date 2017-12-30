#include "WorkOrders.h"
#include "JobBoard.h"
#include "WorkTemplate.h"
#include "../Designations.h"
#include "../ECS/Components/PositionComponent.h"
#include "../ECS/Components/Tags/WorkOrderTag.h"
#include "../ECS/Systems/helpers/WorkOrderHelper.h"
#include "../ECS/Systems/helpers/ItemHelper.h"

namespace JobsBoard
{
	void evaluate_work_order(JobBoard & board, const Entity & e, Coordinates co, JobEvaluatorBase * jt)
	{
		if (!designations->workOrders.empty())
		{
			board.insert(std::make_pair(15, jt));
			return;
		}
		
	}

}


void WorkOrders::init()
{
	JobsBoard::register_job_offer<WorkOrderTag>(JobsBoard::evaluate_work_order);
}

void WorkOrders::update(double duration)
{
	for (auto& e : getEntities())
		work(e, duration);
}

void WorkOrders::work(Entity e, const double& duration)
{
	WorkTemplate<WorkOrderTag> work;
	
	auto& tag = e.getComponent<WorkOrderTag>();
	auto& co = e.getComponent<PositionComponent>().co;
	auto& mov = e.getComponent<MovementComponent>();

	if (tag.step == WorkOrderTag::FIND_WORKSHOP)
	{
		std::unique_ptr<work_order_reaction> job;

		if (!designations->workOrders.empty())
		{
			job = workOrderHelper->find_work_order_reaction(tag);
		}

		if (!job)
		{
			work.cancel_work(e);
			return;
		}


		tag.reaction = *job;
		tag.step = WorkOrderTag::FIND_COMPONENT;
		return;
	}

	else if (tag.step == WorkOrderTag::FIND_COMPONENT)
	{
		std::vector<std::pair<std::size_t, bool>> components;

		bool hasComponents = true;
		for (auto & comp : tag.reaction.components)
		{
			// If the workshop doesn't have all components
			if (!comp.second)
			{
				hasComponents = false;
				tag.current_component = comp.first;

				auto cpos = itemHelper.get_item_location(comp.first);

				if (!cpos)
				{
					work.cancel_work(e);
					return;
				}

				// Set entity destination
				mov.destination = idxToCo(cpos);
				tag.step = WorkOrderTag::GOTO_COMPONENT;
				return;
			}
		}
	}
}
