#include "stdafx.h"
#include "AiArchitecture.h"
#include "JobBoard.h"
#include "Designations.h"
#include "WorkTemplate.h"
#include "../helpers/ItemHelper.h"
#include "../../../Map/Tile.h"
#include "../../../Map/Index.h"
#include "../../Components/Claimed.h"
#include "../EntityPositionCache.h"
#include "../../Components/Sentients/Stats.h"
#include "../DijkstraSystems/DijkstraMapsHandler.h"
#include "../../Messages/update_all_maps_message.h"
#include "../../Messages/recalculate_mining_message.h"
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

void designateArchitecture(designate_architecture_message &msg)
{
	auto co1 = idxToCo(msg.area.first);
	auto co2 = idxToCo(msg.area.second);

	if (co1.z != co2.z)
		return;

	// Normalize Coordinates
	if (co1.x > co2.x)
	{
		int tmp = co1.x;
		co1.x = co2.x;
		co2.x = tmp;
	}
	if (co1.y > co2.y)
	{
		int tmp = co1.y;
		co1.y = co2.y;
		co2.y = tmp;
	}

	std::vector<int> desigs;

	bool possible = true;
	for(int x = co1.x; x <= co2.x; ++x)
		for (int y = co1.y; y <= co2.y; ++y)
		{
			auto idx = getIdx({ x, y, co1.z });

			// Eventually we'll want to be able to designate floors
			// and bridges to be made on tiles we can't stand on.
			// This will do for now though
			if (region::flag({ x, y, co1.z }, region::Flag::CAN_STAND_HERE))
			{
				desigs.emplace_back(idx);
			}
			else
			{
				possible = false;
				break;
			}
		}

	if (possible)
		for (const auto d : desigs)
		{
			designations->architecture.emplace(d, std::make_pair(msg.type, 0.0));
		}
}

void AiArchitecture::init()
{
	JobsBoard::register_job_offer<ArchitectTag>(JobsBoard::evaluate_architecture);

	subscribe<designate_architecture_message>([this](designate_architecture_message &msg)
	{
		designateArchitecture(msg);
	});
}

void AiArchitecture::update(double duration)	// Add in requesting buildings to be done in a particular material type!!!!!!!!
{
	const auto ents = getEntities();

	for (const auto& e : ents)
		doWork(e, duration);
}

void AiArchitecture::doWork(Entity e, const double& duration)
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
			return;

		}, [&tag]()
		{
			// On top of block
			tag.step = ArchitectTag::GRAB_BLOCK;
			return;
		});
	}

	else if (tag.step == ArchitectTag::GRAB_BLOCK) 
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

		if (dist < 2) // Add in two D distance checking>? So we can performa architecture stuff while diagonal from designations
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

		return;
	}

	else if (tag.step == ArchitectTag::BUILD)
	{
		std::size_t bidx = 0;

		// Are we on or adjacent to the build site?
		if (designations->architecture.find(getIdx(co)) != designations->architecture.end())
			bidx = getIdx(co);

		else if (co.y > 0 && designations->architecture.find(getIdx(CO_NORTH)) != designations->architecture.end())
			bidx = getIdx(CO_NORTH);

		else if (co.y < MAP_HEIGHT && designations->architecture.find(getIdx(CO_SOUTH)) != designations->architecture.end())
			bidx = getIdx(CO_SOUTH);

		else if (co.x < MAP_WIDTH && designations->architecture.find(getIdx(CO_EAST)) != designations->architecture.end())
			bidx = getIdx(CO_EAST);

		else if (co.x > 0 && designations->architecture.find(getIdx(CO_WEST)) != designations->architecture.end())
			bidx = getIdx(CO_WEST);

		else if (co.x > 0 && co.y > 0 && designations->architecture.find(getIdx(CO_NORTH_W)) != designations->architecture.end())
			bidx = getIdx(CO_NORTH_W);

		else if (co.x < MAP_WIDTH && co.y > 0 && designations->architecture.find(getIdx(CO_NORTH_E)) != designations->architecture.end())
			bidx = getIdx(CO_NORTH_E);

		else if (co.x < MAP_WIDTH && co.y < MAP_HEIGHT && designations->architecture.find(getIdx(CO_SOUTH_E)) != designations->architecture.end())
			bidx = getIdx(CO_SOUTH_E);

		else if (co.x > 0 && co.y < MAP_HEIGHT && designations->architecture.find(getIdx(CO_SOUTH_W)) != designations->architecture.end())
			bidx = getIdx(CO_SOUTH_W);


		auto find = designations->architecture.find(bidx); 

		// Yay we're in the right spot
		if (find != designations->architecture.end())
		{
			// If this architecture designations progress
			// is less than 100%
			double& progress = find->second.second;
			if (progress < 100.0)
			{
				doWorkTime(duration, progress, DIFFICULTY_NORMAL);
				return;
			}

			auto skillCheck = skillRoll(e.getComponent<Stats>(), "Construction", DIFFICULTY_NORMAL);

			if (skillCheck >= SUCCESS)
			{
				const auto type = find->second.first;

				std::size_t material = 0;

				Entity block = getWorld().getEntity(tag.current_tool);

				if (block.isValid() && block.hasComponent<Item>())
				{
					material = block.getComponent<Item>().material;
				}

				itemHelper.deleteItem(tag.current_tool);
				tag.current_tool = 0;

				region::setFlag(idxToCo(bidx), region::Flag::CONSTRUCTION);

				if (material)
					region::setMaterial(idxToCo(bidx), material);

				switch (type)
				{
				case ArchitectureType::WALL:
					region::makeWall(bidx);
					break;

				case ArchitectureType::FLOOR:
					region::makeFloor(bidx);
					break;

				case ArchitectureType::RAMP:
					region::makeRamp(bidx);
					break;

				case ArchitectureType::UP_STAIRS:
					//region::makeUpStairs(bidx);
					break;

				case  ArchitectureType::DOWN_STAIRS:
					//region::makeDownStairs(bidx);
					break;

				case ArchitectureType::UP_DOWN_STAIRS:
					//region::makeUpDownStairs(bidx);
					break;

				case ArchitectureType::BRIDGE:
					// TODO:
					break;

				default:
					std::cout << "Invalid type architecture attempt!" << "\n";
				}

				// Recalculate paths and render
				region::spot_recalc_paths(idxToCo(bidx));

				designations->architecture.erase(bidx);

				emit(recalculate_mining_message{});
				emit(update_all_maps_message{});
				work.cancel_work(e);
				return;
			}
			// Skill roll FAIL
			else if (skillCheck == FAIL)
				progress = 50.0;		

			// Skill roll CRITICAL_FAIL
			else
				progress = 0.0;

			return;
		}

		else
		{			
			//emit(drop_item_message{ SLOT_CARRYING, e.getId().index, tag.current_tool, co });

			// If not current tool and we're not in the right spot,
			// cancel work
			if (!tag.current_tool)
				work.cancel_work(e);

			// If we have a block but we're not in the right spot, 
			// goto a site
			else
				tag.step = ArchitectTag::GOTO_SITE;
			return;
		}
	}

}
