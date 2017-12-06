#pragma once
#include "../../Systems.h"

class Coordinates;

// Handles the picking up and equipping of 
// items as well as the dropping of them as well
class EquipHandler : public System<Requires<>>
{
public:
	EquipHandler() = default;
	void init();

private:
	void pickupItem(std::size_t entityId, std::size_t itemEid, std::size_t outItemEid);

	void dropItem(std::size_t entityId, std::size_t itemEid, Coordinates co);
};

