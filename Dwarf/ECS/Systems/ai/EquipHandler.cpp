#include "EquipHandler.h"
#include "../../Messages/drop_item_message.h"
#include "../../Messages/pickup_item_message.h"
#include "../Coordinates.h"
#include "../../World.h"
#include "../../Components/PositionComponent.h"
#include "../../Components/ItemStored.h"
#include "../../Messages/entity_moved_message.h"
#include "../../Components/Sentients/Inventory.h"

#include <iostream>

void EquipHandler::init()
{
	subscribe<pickup_item_message>( [this] (pickup_item_message &msg)
	{
		pickupItem(msg.entityId, msg.itemEid, msg.outItem);
	});

	subscribe<drop_item_message>( [this] (drop_item_message &msg)
	{
		dropItem(msg.entityId, msg.itemEid, msg.co);
	});
}

// Need to add in an inventory component for Entities while holding things!~!~!~!~
void EquipHandler::pickupItem(std::size_t entityId, std::size_t itemEid, std::size_t outItemEid)
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
	entity.getComponent<Inventory>();

	// Remove item position component
	item.removeComponent<PositionComponent>();
	
	// Not needed since this is done automatically
	// with onEntityRemove
	//emit(entity_moved_message{ item, EMPTY_COORDINATES, co });

	
	// This could cause issues if first Entity is an item!!!
	// Or if this eid gets reused!!
	if (outItemEid != 0)
		dropItem(entityId, outItemEid, getWorld().getEntity(outItemEid).getComponent<PositionComponent>().co);

	item.activate();
}

// Need to add in an inventory component for Entities while holding things!~!~!~!~
void EquipHandler::dropItem(std::size_t entityId, std::size_t itemEid, Coordinates co)
{
	auto& item = getWorld().getEntity(itemEid);

	auto* stored = &item.getComponent<ItemStored>();

	if (!stored)
		std::cout << "Item being dropped has no ItemStored Component!!!!" << std::endl;

	// Restore the items render component
	auto& rend = item.addComponent<RenderComponent>();
	rend = stored->rend;

	// Restore the items correct positon
	//
	// Perhaps we should remove the position component of stored
	// Entities for better system sorting?
	item.getComponent<PositionComponent>().co = co;

	item.activate();
}
