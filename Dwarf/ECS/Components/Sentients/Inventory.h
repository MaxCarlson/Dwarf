#pragma once

#include "../../Component.h"

enum InventorySlots
{
	INV_TOOL,
	INV_HEAD,
	INV_TORSO,
	INV_LEGS,
	INV_HANDS,
	INV_FEET
};

static const int SENTIENT_INVENTORY_SLOTS = 6;

class Inventory : public Component
{
public:
	// Array holding entity indicies of 
	// equipped items
	std::size_t inventory[SENTIENT_INVENTORY_SLOTS];
};