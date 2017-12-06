#pragma once
#include "../Systems.h"

struct pickup_item_message : public base_message_t
{
	pickup_item_message() = default;
	pickup_item_message(int itemSlot, std::size_t entityId, std::size_t itemEid, std::size_t outItem) : itemSlot(itemSlot), entityId(entityId), itemEid(itemEid), outItem(outItem) {}

	int itemSlot;
	std::size_t entityId;
	std::size_t itemEid;
	std::size_t outItem;
};