#include "../stdafx.h"
#include "EquipHandler.h"
#include "../../Messages/drop_item_message.h"
#include "../../Messages/pickup_item_message.h"
#include "../Coordinates.h"
#include "../../Components/PositionComponent.h"
#include "../../Components/ItemCarried.h"
#include "../../Messages/entity_moved_message.h"
#include "../../Components/Sentients/Inventory.h"
#include "../EntityPositionCache.h"
#include "../../Messages/designate_building_message.h"
#include "../helpers/ItemHelper.h"
#include "../../Components/helpers/building_designation.h"
#include "../../Components/Building.h"
#include "../EntityPositionCache.h"
#include "../../Components/Claimed.h"
#include "../../../Designations.h"
#include "../../Components/ItemStored.h"


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
void EquipHandler::pickupItem(int itemSlot, std::size_t entityId, std::size_t itemEid, std::size_t outItemEid) // Remove the carry slot and just use the ItemCarried Component
{
	auto& item = getWorld().getEntity(itemEid);

	// Store Item
	auto& carried = item.addComponent<ItemCarried>();
	carried.eid = itemEid;

	// Store and remove render Component
	carried.rend = item.getComponent<RenderComponent>();
	item.removeComponent<RenderComponent>();

	// Store item in entities inventory
	auto& entity = getWorld().getEntity(entityId);
	auto& inv = entity.getComponent<Inventory>();

	if (itemSlot < MAX_INVENTORY_SLOTS)				// Get rid of this slot system!
		inv.inventory[itemSlot] = itemEid;
	else
		std::cout << "Other Item Types Not Implemented Yet Error!!" << std::endl;


	// If we're picking up a stored item, flag it as not stored
	if (item.hasComponent<ItemStored>())
		item.removeComponent<ItemStored>();

	// Remove item position component
	// Must first remove from cache
	auto itemCo = item.getComponent<PositionComponent>().co;

	positionCache->removeNode({ itemCo, itemEid });
	item.removeComponent<PositionComponent>();

	if (!item.hasComponent<Claimed>())
		item.addComponent<Claimed>(entityId);


	if (outItemEid != 0)
		dropItem(0, entityId, outItemEid, itemCo);
		//dropItem( finditemSlot(inv, outItemEid), entityId, outItemEid, itemCo); // Item co should be the same as the out item in these cases?

	item.activate();
	getWorld().refresh(); // Should we truly be refreshing here? Not Sure
}

// It looks like we don't need to pass the itemEid when we have entity eid since we can get it
// from searching the inventory?
void EquipHandler::dropItem(int itemSlot, std::size_t entityId, std::size_t itemEid, Coordinates co) // Add a template class for finding if an entity is working with a specific tool in class TAG to see if we can  take their tool away!
{
	if ( itemEid == 0 )
		return;

	auto& item = getWorld().getEntity(itemEid);

	auto* carried = &item.getComponent<ItemCarried>(); 

	// Restore the items render component
	auto& rend = item.addComponent<RenderComponent>(); 

	if (carried)
	{
		rend = carried->rend;
		item.removeComponent<ItemCarried>();
	}

	// Remove item from entities inventory
	getWorld().getEntity(entityId).getComponent<Inventory>().inventory[itemSlot] = 0;

	// Restore the items correct positon
	item.addComponent<PositionComponent>(co);

	if (item.hasComponent<Claimed>())
		item.removeComponent<Claimed>();

	item.activate();
	getWorld().refresh(); // Should we truly be refreshing here? Not Sure
}

int EquipHandler::finditemSlot(const Inventory & einv, std::size_t item) // DELETE
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
		// Set correct quanitity of components needed
		for (int i = 0; i < comp.quantity; ++i)
		{
			const auto compId = itemHelper.claim_item_by_reaction_inp(comp);

			std::cout << "Component [" << comp.tag << "] - Id " << compId << "\n";

			designation.componentIds.push_back(std::make_pair(compId, false));
		}
	}

	auto building = getWorld().createEntity();

	building.addComponent<PositionComponent>(designation.co);
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
			const Coordinates co = { x, y, designation.co.z };
			region::setFlag(co, region::Flag::CONSTRUCTION);

			// Id's must be manually deleted later
			positionCache->addNode({ co, designation.entity_id });
		}

	designations->buildings.push_back(designation);
}
