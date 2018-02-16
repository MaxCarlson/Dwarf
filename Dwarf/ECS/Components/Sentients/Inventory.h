#pragma once

#include "../../Component.h"

enum InventorySlots
{
	SLOT_TOOL,
	SLOT_HEAD,
	SLOT_TORSO,
	SLOT_LEGS,
	SLOT_HANDS,
	SLOT_FEET,

	SLOT_CARRYING,
	SLOT_FOOD,
	SLOT_DRINK
};

static const int MAX_INVENTORY_SLOTS = 9;

class Inventory : public Component
{
public:
	// Array holding entity indicies of 
	// equipped items
	std::size_t inventory[MAX_INVENTORY_SLOTS];

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::base_class<Component>(this), inventory);
	}
};

void deleteItemFromInventory(const Entity& e, Inventory &inv, const int ITEM_SLOT);

CEREAL_REGISTER_TYPE(Inventory);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Inventory);