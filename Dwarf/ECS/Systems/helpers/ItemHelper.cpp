#include "../stdafx.h"
#include "ItemHelper.h"
#include "../Map/Tile.h"
#include "../Raws/Defs/ItemDefs.h"
#include "../Raws/ReactionInput.h"
#include "../ECS/Components/ItemStored.h"
#include "../Raws/Materials.h"
#include "../Raws/Defs/MaterialDef.h"
#include "../Helpers/PositionCache.h"
#include "../ECS/Systems/EntityPositionCache.h"

ItemHelper itemHelper;


void ItemHelper::forEachItem(const std::function<void(Entity)>& func)
{
	for (auto e : getEntities())
	{
		func(e);
	}
}

void ItemHelper::claim_item(Entity& item)
{
	item.addComponent<Claimed>();
	item.activate();
	getWorld().refresh();
}

void ItemHelper::unclaim_item_by_id(std::size_t id)
{
	getWorld().getEntity(id).removeComponent<Claimed>();
	getWorld().getEntity(id).activate();
	getWorld().refresh();
}

void ItemHelper::deleteItem(const std::size_t id)
{
	auto& e = getWorld().getEntity(id);
	auto* pos = &e.getComponent<PositionComponent>();

	if(pos)
		positionCache->removeNode({ pos->co, id });

	e.kill();
	getWorld().refresh();
}

int ItemHelper::get_item_location(std::size_t id)
{
	const auto& ent = getWorld().getEntity(id);
	auto* pos = &ent.getComponent<PositionComponent>();

	if (!pos)
	{
		if (ent.hasComponent<ItemStored>())
		{
			auto storedIn = ent.getComponent<ItemStored>().storedIn;

			if(storedIn)
				return getIdx(getWorld().getEntity(storedIn).getComponent<PositionComponent>().co);
		}
		// Add in carried code? Would probably have to ensure that we don't grab an item from an entity already carrying it		
	}

	else
		return getIdx(pos->co);
	

	return 0;
}

size_t ItemHelper::findClosestItemTypeClaimIt(const Entity & e, const int type, const Coordinates & co)
{
	std::map<double, size_t> distanceMap = { {1000.0, 0} };

	for (const auto& i : getEntities())
	{
		if (i.hasComponent<Claimed>() || !i.getComponent<Item>().catagory.test(type))
			continue;

		distanceMap.insert(std::make_pair(get_3D_distance(i.getComponent<PositionComponent>().co, co), i.getId().index));
	}

	const auto& id = distanceMap.begin()->second;

	if (id > 0)
	{
		getWorld().getEntity(id).addComponent<Claimed>(e.getId().index);
		getWorld().getEntity(id).activate();
	}

	return id;
}

int ItemHelper::num_reaction_input_items(const ReactionInput & react)
{
	int number = 0;
	for (auto e : getEntities())
	{
		const auto& item = e.getComponent<Item>();

		if (react.tag == "any" || item.tag == react.tag)
		{
			if (!e.hasComponent<Claimed>())
			{
				bool count = true;

				// If the required materials don't match..
				if (react.req_material != 0)
					if (react.req_material != item.material)
						count = false;

				if (react.req_material_type != spawn_type_none)
					if (getMaterial(item.material)->matType != react.req_material_type)
						count = false;
				
				if (count)
					++number;
			}
		}
	}

	return number;
}

std::size_t ItemHelper::claim_item_by_reaction_inp(const ReactionInput & react)
{
	for (auto e : getEntities())
	{
		std::size_t claimedItem;

		const auto& item = e.getComponent<Item>();

		if ((react.tag == "any" || item.tag == react.tag) && !e.hasComponent<Claimed>())
		{
			bool claim = true;
			// If the required materials don't match..
			if (react.req_material != 0)
				if (react.req_material != item.material)
					claim = false;

			if (react.req_material_type != spawn_type_none)
				if (getMaterial(item.material)->matType != react.req_material_type)
					claim = false;

			if (claim)
			{
				claimedItem = e.getId().index;
				claim_item(e);

				return claimedItem;
			}			
		}
	}

	return 0;
}


