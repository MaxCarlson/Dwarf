#include "stdafx.h"
#include "AiArchitecture.h"
#include "JobBoard.h"
#include "Designations.h"
#include "WorkTemplate.h"
#include "../../../Map/Tile.h"
#include "../../../Map/Index.h"
#include "../../Components/Claimed.h"
#include "../EntityPositionCache.h"
#include "../DijkstraSystems/DijkstraMapsHandler.h"
#include "../../Messages/drop_item_message.h"
#include "../../Messages/pickup_item_message.h"
#include "../../Messages/designate_architecture_message.h"
#include "../../Messages/block_map_changed_message.h"


namespace JobsBoard
{
	// Skip architecture if there are no designations
	// or we're too far away from either the blocks or the designation
	void evaluate_architecture(JobBoard & board, const Entity & e, Coordinates co, JobEvaluatorBase * jt)
	{
		const auto blockDist = block_map.get(getIdx(co));
		if ( blockDist > MAX_DIJKSTRA_DISTANCE - 1)
			return;

		const auto archDist = architecture_map.get(getIdx(co));
		if ( archDist > MAX_DIJKSTRA_DISTANCE - 1)
			return;
		
		board.insert(std::make_pair(blockDist + archDist, jt));
	}
}

void AiArchitecture::init()
{
	JobsBoard::register_job_offer<ArchitectTag>(JobsBoard::evaluate_architecture);
}

void AiArchitecture::update(double duration)
{
	const auto& ents = getEntities();

	for (const auto& e : ents)
		doWork(e);
}

void AiArchitecture::doWork(Entity e)
{
	WorkTemplate<ArchitectTag> work;

	auto& tag = e.getComponent<ArchitectTag>();
	auto& co  = e.getComponent<PositionComponent>().co;
	auto& mov = e.getComponent<MovementComponent>();

	if (tag.step == ArchitectTag::GOTO_BLOCK)
	{
		work.followMap(block_map, e, co, [&e, &work]()
		{
			// On path failure
			work.cancel_work(e);

		}, [&tag]()
		{
			// On top of block
			tag.step = ArchitectTag::GRAB_BLOCK;
		});
		return;
	}

	else if (tag.step == ArchitectTag::GRAB_BLOCK) // Add in requesting buildings to be done in a particular material type!!!!!!!!
	{
		const auto& atPos = positionCache->get_location(getIdx(co));

		std::size_t blockId = 0;
		for (const auto& id : atPos)
		{
			auto& b = getWorld().getEntity(id);

			if (b.hasComponent<Claimed>() || !b.hasComponent<Item>())
				continue;

			if (b.getComponent<Item>().tag == "block")
			{
				blockId = id;
				break;
			}
		}

		if (blockId == 0)
		{
			work.cancel_work(e);
			return;
		}
			
		emit(pickup_item_message{ SLOT_CARRYING, e.getId().index, blockId, tag.current_tool });
		emit(block_map_changed_message{});

		tag.current_tool = blockId;
		tag.step = ArchitectTag::GOTO_SITE;
		return;
	}

	else if (tag.step == ArchitectTag::GOTO_SITE)
	{
		if (mov.progress || !mov.path.empty())
			return;

		const int idx = getIdx(co);
		const auto dist = architecture_map.get(idx);

		if (dist > MAX_DIJKSTRA_DISTANCE - 1)
		{
			emit(drop_item_message{ SLOT_CARRYING, e.getId().index, tag.current_tool, co });
			work.cancel_work(e);
			return;
		}

		if (dist < 2)
		{
			tag.step = ArchitectTag::BUILD;
			return;
		}

		auto dest = architecture_map.findDestination(co);

		// Is this step neccasary?
		if (dest != EMPTY_COORDINATES)
			mov.path.emplace_back(dest);

		else
		{
			emit(drop_item_message{ SLOT_CARRYING, e.getId().index, tag.current_tool, co });
			work.cancel_work(e);
		}
		
	}

	else if (tag.step == ArchitectTag::BUILD)
	{

	}

}
