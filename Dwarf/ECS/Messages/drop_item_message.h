#pragma once
#include "../Systems.h"
#include "../Coordinates.h"

struct drop_item_message : public base_message_t
{
	drop_item_message() = default;
	drop_item_message(std::size_t entityId, std::size_t itemEid, Coordinates co) : entityId(entityId), itemEid(itemEid), co(co) {}

	std::size_t entityId;
	std::size_t itemEid;
	const Coordinates co;
};