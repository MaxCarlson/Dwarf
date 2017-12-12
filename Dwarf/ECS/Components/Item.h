#pragma once

#include "../Component.h"

enum ItemType { CLOTHING, ITEM };

class Item : public Component
{
public:
	Item() = default;
	Item(const std::string name) : itemName(name) {}

	std::string itemName;
	std::string itemTag;
	std::bitset<64> catagory;

	ItemType type = ITEM;

	bool claimed = false;
	int stackSize = 1;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::base_class<Component>(this), itemName, itemTag, catagory, type, claimed, stackSize);
	}
};

CEREAL_REGISTER_TYPE(Item);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Item);