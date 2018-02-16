#include "Inventory.h"
#include "Globals\GlobalWorld.h"

void deleteItemFromInventory(const Entity& e, Inventory & inv, const int ITEM_SLOT)
{
	auto item = inv.inventory[ITEM_SLOT];

	if (item > 0)
	{
		world.getEntity(item).kill();
	}

	inv.inventory[ITEM_SLOT] = 0;
}
