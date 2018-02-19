#pragma once
#include "../../Systems.h"

struct Coordinates;
class Inventory;
struct designate_building_message;

// Handles the picking up and equipping of 
// items as well as the dropping of them as well
class EquipHandler : public System<Requires<>>
{
public:
	EquipHandler() = default;
	void init();
	void update(const double duration);

private:
	void pickupItem(int itemType, std::size_t entityId, std::size_t itemEid, std::size_t outItemEid);

	void dropItem(int itemType, std::size_t entityId, std::size_t itemEid, Coordinates co);
};

