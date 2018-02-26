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
#include "../../Components/ItemStored.h"
#include "Helpers\MessageQueue.h"
#include "ECS\Components\Fighting\MeleeWeapon.h"
#include "ECS\Components\Fighting\CombatBase.h"

//MessageQueue<pickup_item_message> pickups;
//MessageQueue<drop_item_message> dropOffs;

void EquipHandler::init()
{
	subscribe<pickup_item_message>([this](pickup_item_message &msg)
	{
		pickupItem(msg.itemSlot, msg.entityId, msg.itemEid, msg.outItem );
	});

	subscribe<drop_item_message>([this](drop_item_message &msg)
	{
		dropItem( msg.itemSlot, msg.entityId, msg.itemEid, msg.co );
	});
}

void EquipHandler::update(const double duration)
{
}

void EquipHandler::pickupItem(int itemSlot, std::size_t entityId, std::size_t itemEid, std::size_t outItemEid) 
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

	if (outItemEid == 0)
		outItemEid = inv.inventory[itemSlot];

	if (itemSlot < MAX_INVENTORY_SLOTS)				
		inv.inventory[itemSlot] = itemEid;
	else
		std::cout << "Inventory Index Error!" << std::endl;


	// If we're picking up a stored item, flag it as not stored
	if (item.hasComponent<ItemStored>())
		item.removeComponent<ItemStored>();

	if (entity.hasComponent<CombatBase>() && item.hasComponent<MeleeWeapon>()) // || RangedWeapon .. etc
	{
		// Calculate our new combat base with this item if it's a weapon
		calculateCombatBase(entity, itemEid);
	}

	// Remove item position component
	// Must first remove from cache
	auto itemCo = item.getComponent<PositionComponent>().co;

	positionCache->removeNode({ itemCo, itemEid });
	item.removeComponent<PositionComponent>();

	if (!item.hasComponent<Claimed>())
		item.addComponent<Claimed>(entityId);


	if (outItemEid != 0)
		dropItem(itemSlot, entityId, outItemEid, itemCo);

	item.activate();
}

void EquipHandler::dropItem(int itemSlot, std::size_t entityId, std::size_t itemEid, Coordinates co) 
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

	auto entity = getWorld().getEntity(entityId);
	if (entity.hasComponent<CombatBase>() && item.hasComponent<MeleeWeapon>()) // || RangedWeapon .. etc
	{
		// Calculate our new combat base once we've dropped this weapon
		calculateCombatBase(entity, 0);
	}

	// Remove item from entities inventory
	entity.getComponent<Inventory>().inventory[itemSlot] = 0;

	// Restore the items correct positon
	item.addComponent<PositionComponent>(co);

	if (item.hasComponent<Claimed>())
		item.removeComponent<Claimed>();

	item.activate();
}
