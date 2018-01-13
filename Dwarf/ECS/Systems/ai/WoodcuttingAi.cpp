#include "stdafx.h"
#include "WoodcuttingAi.h"
#include "JobBoard.h"
#include "WorkTemplate.h"
#include "../helpers/ItemHelper.h"
#include "../../../Designations.h"
#include "../../../Map/Tile.h"
#include "../../Components/Sentients/Stats.h"
#include "../../Messages/drop_item_message.h"
#include "../../Messages/axemap_changed_message.h"
#include "../../../Raws/Defs/ItemDefs.h"
#include "../helpers/PathFinding.h"

namespace JobsBoard
{
	void evaluate_woodcutting(JobBoard & board, const Entity & e, Coordinates co, JobEvaluatorBase * jt)
	{
		if (designations->woodcutting.empty())
			return;

		auto axeDist = axe_map.get(getIdx(co));

		if (axeDist > MAX_DIJKSTRA_DISTANCE - 1)
			return;

		int i = 0;

		auto treeDist = std::numeric_limits<double>().max();

		// Find the closest tree to Entity
		for (const auto& des : designations->woodcutting)
		{
			const auto d = region::get_3D_distance(co, des.second);

			if (d < treeDist)
				treeDist = d;		
		}

		board.insert(std::make_pair(axeDist + treeDist, jt));
	}
}

void WoodcuttingAi::init()
{
	JobsBoard::register_job_offer<LumberjacTag>(JobsBoard::evaluate_woodcutting);
}

void WoodcuttingAi::update(const double duration)
{
	for (auto e : getEntities())
	{
		doWork(e, duration);
	}
}

inline void fillTreeTargets(std::array<Coordinates, 8> &targets, const Coordinates co)
{
	targets[0] = CO_NORTH;
	targets[1] = CO_NORTH_E;
	targets[2] = CO_NORTH_W;
	targets[3] = CO_SOUTH;
	targets[4] = CO_SOUTH_E;
	targets[5] = CO_SOUTH_W;
	targets[6] = CO_EAST;
	targets[7] = CO_WEST;
}

void WoodcuttingAi::doWork(Entity & e, const double & duration)
{
	WorkTemplate<LumberjacTag> work;

	auto& co  = e.getComponent<PositionComponent>().co;
	auto& mov = e.getComponent<MovementComponent>();
	auto& tag = e.getComponent<LumberjacTag>();

	if (tag.step == LumberjacTag::GET_AXE)
	{
		work.followMap(axe_map, e, co, [&e, &work]()
		{
			// On work cancel
			work.cancel_work(e);
			return;
		}, [&tag, &e, &work, &co]
		{
			// On reaching tool
			work.pickup_tool<axemap_changed_message>(e, co, TOOL_CHOPPING, tag.current_axe, [&work, &e]()
			{
				// On pickup failure
				work.cancel_work(e);
				return;
			}, [&tag]
			{
				// On pickup success
				tag.step = LumberjacTag::FIND_TREE;
				return;
			});

		});
	}

	else if (tag.step == LumberjacTag::FIND_TREE)
	{
		if (designations->woodcutting.empty())
		{
			emit(drop_item_message{ SLOT_TOOL, e.getId().index, tag.current_axe, co });
			emit(axemap_changed_message{});
			work.cancel_work(e);
			return;
		}

		// Set claimed trees
		std::set<std::size_t> claimedTrees;
		for (const auto & ents : getEntities())
		{
			if (e != ents && e.getComponent<LumberjacTag>().treeId > 0)
				claimedTrees.insert(e.getComponent<LumberjacTag>().treeId);
		}

		// Find the closest tree that's not already claimed
		std::size_t treeId = 0;
		auto dist = std::numeric_limits<double>().max();

		for (auto& des : designations->woodcutting)
		{
			auto find = claimedTrees.find(des.first);

			if (find != claimedTrees.end())
			{
				const auto d = region::get_3D_distance(co, des.second);

				if (d < dist)
				{
					dist = d;
					treeId = des.first;
				}
			}
		}

		if (treeId == 0)
		{
			emit(drop_item_message{ SLOT_TOOL, e.getId().index, tag.current_axe, co });
			emit(axemap_changed_message{});
			work.cancel_work(e);
			return;
		}

		auto treePos = designations->woodcutting.find(treeId)->second;

		tag.treeId = treeId;
		tag.treeCo = treePos;

		// Target all 8 squares around the tree
		std::array<Coordinates, 8> targets;

		fillTreeTargets(targets, treePos);

		// And try to find a path to any of them
		for (int i = 0; i < 8; ++i)
		{
			auto path = findPath(co, targets[i]);

			if (!path->failed)
			{
				mov.path = path->path;
				break;
			}	
		}

		// Pathing success
		if (!mov.path.empty())
		{
			tag.step = LumberjacTag::GOTO_SITE;
			return;
		}
		
		// Pathfinding to tree failed. Cancel work
		emit(drop_item_message{ SLOT_TOOL, e.getId().index, tag.current_axe, co });
		emit(axemap_changed_message{});
		work.cancel_work(e);
		return;
	}

	else if (tag.step == LumberjacTag::GOTO_SITE)
	{
		if (mov.progress || !mov.path.empty())
			return;

		bool z = tag.treeCo.z == co.z;
		auto dist = region::get_2D_distance(co, tag.treeCo);

		// We're close enough to start chopping!!
		if (z && dist < 2)
		{
			tag.step = LumberjacTag::CHOP;
			return;
		}

		// We're not close enough and we have no path,
		// try and find another path
		tag.step = LumberjacTag::FIND_TREE;
		return;
	}

	else if (tag.step = LumberjacTag::CHOP)
	{

	}
}
