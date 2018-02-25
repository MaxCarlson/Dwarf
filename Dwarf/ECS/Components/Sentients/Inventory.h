#pragma once

#include "../../Component.h"

enum InventorySlots
{
	SLOT_TOOL,
	SLOT_CARRYING,
	SLOT_FOOD,
	SLOT_DRINK,
	SLOT_HEAD,
	SLOT_TORSO,
	SLOT_LEGS,
	SLOT_HANDS,
	SLOT_FEET,
	SLOT_WEAPON,
	SLOT_SHIELD
};

static const int MAX_INVENTORY_SLOTS = 11;

class Inventory : public Component
{
public:
	// Array holding entity indicies of 
	// equipped items
	size_t inventory[MAX_INVENTORY_SLOTS];

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(inventory);
	}
};

void deleteItemFromInventory(const Entity& e, Inventory &inv, const int ITEM_SLOT);

CEREAL_REGISTER_TYPE(Inventory);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Inventory);