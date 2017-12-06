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

static const int MAX_INVENTORY_SLOTS = 6;

class Inventory : public Component
{
public:
	// Array holding entity indicies of 
	// equipped items
	std::size_t inventory[MAX_INVENTORY_SLOTS];
};