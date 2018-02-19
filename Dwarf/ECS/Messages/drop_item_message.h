#pragma once
#include "../Systems.h"
#include "../Coordinates.h"

struct drop_item_message : public base_message_t
{
	drop_item_message() = default;
	drop_item_message(int itemSlot, std::size_t entityId, std::size_t itemEid, Coordinates co, bool unclaim = true) 
		: itemSlot(itemSlot), entityId(entityId), itemEid(itemEid), co(co), unclaim(unclaim) {}

	int itemSlot;
	std::size_t entityId;
	std::size_t itemEid;
	const Coordinates co;
	bool unclaim = true;
};