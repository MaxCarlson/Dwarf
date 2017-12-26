#include "EquipHandler.h"
#include "../../Messages/drop_item_message.h"
#include "../../Messages/pickup_item_message.h"
#include "../Coordinates.h"
#include "../../World.h"
#include "../../Components/PositionComponent.h"
#include "../../Components/ItemStored.h"
#include "../../Messages/entity_moved_message.h"
#include "../../Components/Sentients/Inventory.h"
#include "../EntityPositionCache.h"
#include "../../Messages/designate_building_message.h"
#include "../helpers/ItemHelper.h"
#include "../../Components/helpers/building_designation.h"
#include "../../Components/Building.h"

#include <iostream>

void EquipHandler::init()
{
	subscribe<pickup_item_message>( [this] (pickup_item_message &msg)
	{
		pickupItem(msg.itemSlot, msg.entityId, msg.itemEid, msg.outItem);
	});

	subscribe<drop_item_message>( [this] (drop_item_message &msg)
	{
		dropItem(msg.itemSlot, msg.entityId, msg.itemEid, msg.co);
	});

	subscribe<designate_building_message>([this](designate_building_message & msg)
	{
		designateBuilding(msg);
	});
}

// Need to add in an inventory component for Entities while holding things!~!~!~!~
void EquipHandler::pickupItem(int itemSlot, std::size_t entityId, std::size_t itemEid, std::size_t outItemEid)
{
	auto& item = getWorld().getEntity(itemEid);

	// Store Item
	auto& stored = item.addComponent<ItemStored>();
	stored.eid = itemEid;

	// Store and remove render Component
	stored.rend = item.getComponent<RenderComponent>();
	item.removeComponent<RenderComponent>();

	// Store item in entities inventory
	auto& entity = getWorld().getEntity(entityId);
	auto& inv = entity.getComponent<Inventory>();

	if (itemSlot < MAX_INVENTORY_SLOTS)
		inv.inventory[itemSlot] = itemEid;
	else
		std::cout << "Other Item Types Not Implemented Yet Error!!" << std::endl;



	// Remove item position component
	// Must first remove from caching
	// perhaps this isn't worth it
	auto itemCo = item.getComponent<PositionComponent>().co;

	deletePositionCache(item, itemCo);
	item.removeComponent<PositionComponent>();

	// This could cause issues if first Entity is an item!!!
	// Or if this eid gets reused!! ~~ Perhaps start at Entity id 1?
	if (outItemEid != 0)
		dropItem( finditemSlot(inv, outItemEid), entityId, outItemEid, itemCo); // Item co should be the same as the out item in these cases?

	item.activate();
}

// It looks like we don't need to pass the itemEid when we have entity eid since we can get it
// from searching the inventory?
void EquipHandler::dropItem(int itemSlot, std::size_t entityId, std::size_t itemEid, Coordinates co)
{
	auto& item = getWorld().getEntity(itemEid);
	auto& entity = getWorld().getEntity(entityId);

	// Reset inventory slot
	auto& inv = entity.getComponent<Inventory>();
	inv.inventory[itemSlot] = 0;

	auto* stored = &item.getComponent<ItemStored>();

	if (!stored)
		std::cout << "Item being dropped has no ItemStored Component!!!!" << std::endl;

	// Restore the items render component
	auto& rend = item.addComponent<RenderComponent>();
	rend = stored->rend;

	// Restore the items correct positon
	item.addComponent<PositionComponent>(co);

	item.activate();
}

int EquipHandler::finditemSlot(const Inventory & einv, std::size_t item)
{
	for (auto i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	{
		if (einv.inventory[i] == item)
			return i;
	}
}

void EquipHandler::designateBuilding(designate_building_message & msg)
{
	building_designation designation;

	designation.tag = msg.building.tag;
	designation.name = msg.building.name;
	designation.co = idxToCo(msg.idx);
	designation.width = msg.building.width;
	designation.height = msg.building.height;
	designation.components = msg.building.components;
	

	// Find building components
	for (const auto& comp : msg.building.components)
	{
		const auto compId = itemHelper.claim_item_by_reaction_inp(comp);

		std::cout << "Component [" << comp.tag << "] - Id " << compId << "\n";

		designation.componentIds.push_back(std::make_pair(compId, false));
	}

	auto building = getWorld().createEntity();

	building.addComponent<PositionComponent>(Coordinates{ idxToCo(msg.idx) });
	building.addComponent<Building>( Building{ designation.tag, designation.width, designation.height, false, msg.building.charCodes });

	designation.entity_id = building.getId().index;

	// Adjust center for 3 tile buildings
	int sx = designation.co.x;
	int sy = designation.co.y;
	if (designation.width == 3) --sx;
	if (designation.height == 3) --sy;

	for(int x = sx; x < sx + designation.width; ++x)
		for (int y = sy; y < sy + designation.height; ++y)
		{

		}
}
