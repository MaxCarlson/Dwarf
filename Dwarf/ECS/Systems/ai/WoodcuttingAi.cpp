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
#include "../../../Raws/raws.h"
#include "../../../Raws/Defs/ItemDefs.h"
#include "../../../Raws/Materials.h"
#include "../helpers/PathFinding.h"
#include "ECS\Components\Sentients\AiWorkComponent.h"

static const std::string jobSkill = "wood_cutting";

namespace JobsBoard
{
	void evaluate_woodcutting(JobBoard & board, const Entity & e, AiWorkComponent &prefs, const Coordinates& co, JobEvaluatorBase * jt)
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
			const auto d = get_3D_distance(co, des.second);

			if (d < treeDist)
				treeDist = d;		
		}

		prefereceAndSubmitJob(board, prefs, jt, jobSkill, static_cast<int>(treeDist) + axeDist);
	}
}

void WoodcuttingAi::init()
{
	JobsBoard::register_job_offer<LumberjacTag>(JobsBoard::evaluate_woodcutting);
}

void WoodcuttingAi::update(const double duration)
{
	auto ents = getEntities();
	for (auto e : ents)
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

		return;
	}

	else if (tag.step == LumberjacTag::FIND_TREE)
	{
		if (designations->woodcutting.empty())
		{
			tag.step = LumberjacTag::DROP_AXE;
			return;
		}

		// Set claimed trees
		std::set<std::size_t> claimedTrees;
		for (const auto & ents : getEntities())
		{
			if (e != ents && ents.hasComponent<LumberjacTag>() && ents.getComponent<LumberjacTag>().treeId > 0)
				claimedTrees.insert(ents.getComponent<LumberjacTag>().treeId);
		}

		// Find the closest tree that's not already claimed
		std::size_t treeId = 0;
		auto dist = std::numeric_limits<double>().max();

		for (auto& des : designations->woodcutting)
		{
			auto find = claimedTrees.find(des.first);

			// If tree isn't claimed..
			if (find == claimedTrees.end())
			{
				const auto d = get_3D_distance(co, des.second);

				if (d < dist)
				{
					dist = d;
					treeId = des.first;
				}
			}
		}

		if (treeId == 0)
		{
			tag.step = LumberjacTag::DROP_AXE;
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
		tag.step = LumberjacTag::DROP_AXE;
		return;
	}

	else if (tag.step == LumberjacTag::GOTO_SITE)
	{
		if (mov.progress || !mov.path.empty())
			return;

		bool z = tag.treeCo.z == co.z;
		auto dist = get_2D_distance(co, tag.treeCo);

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

	else if (tag.step == LumberjacTag::CHOP)
	{
		auto find = designations->woodcutting.find(tag.treeId);

		if (find == designations->woodcutting.end())
		{
			tag.step = LumberjacTag::FIND_TREE;
			return;
		}

		auto& stats = e.getComponent<Stats>();

		int treeHp = region::treeHealth(tag.treeId);

		if (treeHp > 0)
		{
			const auto dmg = doWorkDamage(stats, jobSkill);

			region::damageTree(tag.treeId, dmg);
		}

		else
		{
			giveWorkXp(stats, jobSkill, DIFFICULTY_MODERATE);

			Coordinates tco = tag.treeCo;

			// Square coordinates so we don't go out of bounds
			//
			// Possibly in tree files somewhere add a max width, height, and depth to tree's
			// so we can use constants here instead of the 12's
			int x = tco.x - 12 >= 0 ? tco.x - 12 : 0;
			int y = tco.y - 12 >= 0 ? tco.y - 12 : 0;
			int z = tco.z - 12 >= 0 ? tco.z - 12 : 0;

			int xl = tco.x + 12 < MAP_WIDTH  - 1 ? tco.x + 12 : MAP_WIDTH  - 1;	
			int yl = tco.y + 12 < MAP_HEIGHT - 1 ? tco.y + 12 : MAP_HEIGHT - 1;			
			int zl = tco.z + 12 < MAP_DEPTH  - 1 ? tco.z + 12 : MAP_DEPTH  - 1;

			int numLogs = 0;
			int stumpIdx = 0;
			int lowestZ = 10000;

			// Search the area around the tree and 
			// replace it with empty space
			for (int X = x; X < xl; ++X){
				for (int Y = y; Y < yl; ++Y){
					for (int Z = z; Z < zl; ++Z)
					{
						const int idx = getIdx({ X, Y, Z });

						if (region::treeId(idx) == tag.treeId)
						{
							if (z < lowestZ)
							{
								lowestZ = Z;
								stumpIdx = idx;
							}

							region::makeEmptySpace(idx);
							region::tile_recalc(idxToCo(idx));
							region::spot_recalc_paths(idxToCo(idx)); // Examine this area if there are issues with pathing or render after cutting trees

							++numLogs;
						}
					}
				}
			}



			// Using this means only the lowest section of the tree can be designated
			region::makeFloor(getIdx(tag.treeCo)); 
			region::tile_recalc(tag.treeCo);

			numLogs = (numLogs / 20) + 1;
			std::cout << "spawning " << numLogs << " logs\n";

			for (int i = 0; i < numLogs; ++i)
			{
				spawnItemOnGround("wood_log", getMaterialIdx("wood"), tag.treeCo, SpawnColor::ITEM_COLOR, NORMAL_QUALITY);
			}

			// Recalcuate paths
			for (int x = -10; x < 10; ++x)
				for (int y = -10; y < 10; ++y)
					for (int z = -2; z < 12; ++z)
						region::tile_recalc({ tag.treeCo.x + x, tag.treeCo.y + y, tag.treeCo.z + z });

			region::deleteTree(tag.treeId);

			designations->woodcutting.erase(tag.treeId);

			tag.step = LumberjacTag::DROP_AXE;
			return;		
		}
	}

	else if (tag.step == LumberjacTag::DROP_AXE)
	{
		emit(drop_item_message{ SLOT_TOOL, e.getId().index, tag.current_axe, co });
		emit(axemap_changed_message{});
		work.cancel_work(e);
		return;
	}
}
