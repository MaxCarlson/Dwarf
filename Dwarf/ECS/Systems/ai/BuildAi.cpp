#include "BuildAi.h"
#include "JobBoard.h"
#include "WorkTemplate.h"
#include "../Raws/DefInfo.h"
#include "../Designations.h"
#include "../Raws/Buildings.h"
#include "../Raws/Defs/BuildingDef.h"
#include "../../../Map/Tile.h"
#include "../helpers/ItemHelper.h"
#include "../helpers/PathFinding.h"
#include "ECS\Components\Sentients\AiWorkComponent.h"
#include "../../Components/Building.h"
#include "../../Components/PositionComponent.h"
#include "../../Components/Tags/BuilderTag.h"
#include "../../Components/MovementComponent.h"
#include "../../Components/Sentients/Stats.h"
#include "../../Components/Sentients/Inventory.h"
#include "../../Messages/pickup_item_message.h"
#include "../../Messages/drop_item_message.h"

#include <utility>

static const std::string jobSkill = "construction";

namespace JobsBoard
{
	void evaluate_building(JobBoard & board, const Entity & e, AiWorkComponent &prefs, const Coordinates& co, JobEvaluatorBase * jt)
	{
		if (designations->buildings.empty())
			return;

		auto building_d = designations->buildings.back();
		auto id = building_d.entity_id; 

		auto building = e.getWorld().getEntity(id);

		auto* pos = &building.getComponent<PositionComponent>().co;

		if (pos)
		{
			auto distance = static_cast<int>(get_3D_distance(co, *pos));

			prefereceAndSubmitJob(board, prefs, jt, jobSkill, distance);
		}
	}
}

void BuildAi::init()
{
	JobsBoard::register_job_offer<BuilderTag>(JobsBoard::evaluate_building);
}

void BuildAi::update(const double duration)
{
	for (auto e : getEntities())
	{
		doBuild(e, duration);
	}
}

void BuildAi::doBuild(const Entity & e, const double & duration)
{
	WorkTemplate<BuilderTag> work;

	auto& tag = e.getComponent<BuilderTag>();
	auto& co = e.getComponent<PositionComponent>().co;
	auto& mov = e.getComponent<MovementComponent>();

	if (tag.step == BuilderTag::FIND_BUILDING)
	{
		if (designations->buildings.empty())
		{
			work.cancel_work(e);
			return;
		}

		tag.buildingTarget = designations->buildings.back();

		// Set difficulty now so we have fast lookup later
		tag.difficulty = getBuilding(tag.buildingTarget.tag)->skill_required.second;

		designations->buildings.pop_back(); 
		
		tag.step = BuilderTag::FIND_COMPONENT;
	}

	else if (tag.step == BuilderTag::FIND_COMPONENT)
	{
		bool hasComps = true;

		// Loop through component types
		for (auto& ctype : tag.buildingTarget.components)
		{
			// Loop through quantity of components
			for (int i = 0; i < ctype.quantity; ++i)
			{
				auto& component = tag.buildingTarget.componentIds[i];

				// If component hasn't been placed by building yet..
				if (!component.second)
				{
					hasComps = false;
					tag.current_component = component.first;

					auto pos = itemHelper.get_item_location(component.first);
					if (!pos)
					{
						// We failed to find a component for this building
						// at first, let's try again ~~ Should we remove this code from EquipHandler and put it solely in here?
						if (component.first == 0)
						{
							const auto id = itemHelper.claim_item_by_reaction_inp(ctype);
							component = std::make_pair(id, false);
						}

						designations->buildings.push_back(tag.buildingTarget);
						work.cancel_work(e);
						return;
					}

					// Set destination, pathing will be done later
					//mov.destination = idxToCo(pos);
					auto path = findPath(co, idxToCo(pos));

					if (path->failed)
					{
						designations->buildings.push_back(tag.buildingTarget);
						work.cancel_work(e);
						return;
					}

					mov.path = path->path;

					component.second = true;
					tag.step = BuilderTag::GOTO_COMPONENT;
					return;
				}
			}
		}

		if (hasComps)
			tag.step = BuilderTag::BUILD_BUILDING;

		return;
	}

	else if (tag.step == BuilderTag::GOTO_COMPONENT)
	{
		// Don't interrupt movement
		if (mov.progress || !mov.path.empty())
			return;

		// We're on top of the item!
		if (getIdx(co) == itemHelper.get_item_location(tag.current_component))
		{
			tag.step = BuilderTag::GRAB_COMPONENT;
			return;
		}

		// Path didn't work out, try again. Possibly revisit this if there are issues
		tag.step = BuilderTag::FIND_COMPONENT;
		return;
	}

	else if (tag.step == BuilderTag::GRAB_COMPONENT)
	{
		emit(pickup_item_message{ InventorySlots::SLOT_CARRYING, e.getId().index, tag.current_component, 0 });
		tag.step = BuilderTag::GOTO_BUILDING;
	}

	else if (tag.step == BuilderTag::GOTO_BUILDING)
	{
		if (mov.progress || !mov.path.empty())
			return;

		auto building = getWorld().getEntity(tag.buildingTarget.entity_id);

		auto* pos = &building.getComponent<PositionComponent>().co;

		if (!pos)
		{
			emit(drop_item_message{ SLOT_CARRYING, e.getId().index, tag.current_component, co });
			work.cancel_work(e);
			return;
		}

		const auto dist = get_2D_distance(co, *pos);
		const bool zeq = co.z == pos->z;

		// Drop component and tell building it has the component
		if (co == *pos || (zeq && dist < 1.41))
		{
			for (auto & component : tag.buildingTarget.componentIds)
				if (tag.current_component == component.first)
					component.second = true;

			tag.step = BuilderTag::DROP_COMPONENT;
			return;
		}	

		if (mov.path.empty())
		{
			auto path = findPath(co, *pos);
			
			if (path->failed)
			{
				emit(drop_item_message{ SLOT_CARRYING, e.getId().index, tag.current_component, co });
				work.cancel_work(e);
				return;
			}

			mov.path = path->path;
			return;
		}
	}

	else if (tag.step == BuilderTag::DROP_COMPONENT)
	{
		emit(drop_item_message{ SLOT_CARRYING, e.getId().index, tag.current_component, co });

		// Stop gap measure. Keep items claimed for a building. If the building is un-designated
		// implement unclaiming of items
		itemHelper.claim_item(getWorld().getEntity(tag.current_component));

		tag.current_component = 0;
		tag.step = BuilderTag::FIND_COMPONENT;
		return;
	}

	else if (tag.step == BuilderTag::BUILD_BUILDING)
	{
		auto& stats = e.getComponent<Stats>();

		if (tag.buildingTarget.progress < 2000.0) // Add in skill levels affecting time taken!!
		{
			doWorkTime(stats, jobSkill, duration, tag.buildingTarget.progress);
			
			return;
		}

		auto target = getBuilding(tag.buildingTarget.tag);
		if (target == nullptr)
			throw std::runtime_error("Building tag not found!");

		const int difficulty = target->skill_required.second;

		giveWorkXp(stats, jobSkill, difficulty);

		auto& buildingEntity = getWorld().getEntity(tag.buildingTarget.entity_id);

		auto& building = buildingEntity.getComponent<Building>();

		// Check every component for validity
		for (auto& compId : tag.buildingTarget.componentIds)
		{
			const auto ent = getWorld().getEntity(compId.first);

			if (!ent.isValid() || !ent.hasComponent<Item>())
			{
				designations->buildings.push_back(tag.buildingTarget);
				std::cout << "Invalid component for building - building id: " << tag.buildingTarget.entity_id
					<< " componnent id:" << compId.first << "\n";
				work.cancel_work(e);
				return;
			}
		}

		// Set materials for building and delete componenets 
		for (auto& compId : tag.buildingTarget.componentIds)
		{
			const auto comp = getWorld().getEntity(compId.first);

			const std::string comptag = comp.getComponent<Item>().tag;
			std::size_t material = comp.getComponent<Item>().material;

			itemHelper.deleteItem(compId.first);

			building.materials.push_back(std::make_pair(comptag, material));
		}


		building.complete = true;

		// Just used for filtering
		buildingEntity.addComponent<RenderComponent>();

		buildingEntity.activate();

		// Update availible reactions for buildings if this is a new Building type
		defInfo->updateBuildingReactions(building.tag);

		// Add code for building provides once added in 


		work.cancel_work(e);
		return;
	}
}
