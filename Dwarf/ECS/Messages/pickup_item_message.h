#pragma once
#include "../Systems.h"

struct pickup_item_message : public base_message_t
{
	pickup_item_message() = default;
	pickup_item_message(std::size_t itemEid, std::size_t outTool) : itemEid(itemEid), outItem(outItem) {}

	std::size_t itemEid;
	std::size_t outItem;
};