#pragma once

#include "../Component.h"

class Item : public Component
{
public:
	Item() = default;
	Item(const std::string name);
	Item(std::bitset<64> catagory);

	std::string itemName;
	std::bitset<64> catagory;

	bool claimed = false;
};