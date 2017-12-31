#include "../../../stdafx.h"
#include "HaulingSystem.h"
#include "WorkTemplate.h"
#include "JobBoard.h"
#include "StockpileSystem.h"
#include "../../Components/PositionComponent.h"
#include "../../Components/Tags/HaulingTag.h"


namespace JobsBoard
{
	void evaluate_hauling(JobBoard & board, const Entity & e, Coordinates co, JobEvaluatorBase * jt)
	{
		// No items to be hauled :(
		if (storeableItems.empty())
			return;

		// first storable co;
		const auto co2 = idxToCo(storeableItems.back().desination);

		// Distance to item
		const auto idist = region::get_3D_distance(co, co2);

		// Total distance
		const int distance = int( idist + region::get_3D_distance(co2, e.getWorld().getEntity(storeableItems.back().itemId).getComponent<PositionComponent>().co) );


		// Add some other prefrences aside from distance,
		// Possibly make less skilled workers prefer hauling?
		// Low priority?

		board.insert(std::make_pair(distance, jt));
	}
}

void HaulingSystem::init()
{
	JobsBoard::register_job_offer<HaulingTag>(JobsBoard::evaluate_hauling);
}

void HaulingSystem::update(const double duration)
{
	
	for (auto& e : getEntities())
	{
		WorkTemplate<HaulingTag> work;
		auto& co = e.getComponent<PositionComponent>().co;
		auto& tag = e.getComponent<HaulingTag>();
	}
}
