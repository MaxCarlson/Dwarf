#pragma once

#include "../Component.h"

class Item : public Component
{
public:
	Item() = default;
	Item(std::string& name, std::string& tag, std::bitset<64>& catagory, std::size_t material, int stackSize) 
		: name(name), tag(tag), catagory(catagory), material(material), stackSize(stackSize) {}

	std::string name;
	std::string tag;
	std::bitset<64> catagory;
	std::size_t material;
	int stackSize = 1;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(name, tag, catagory, material, stackSize);
	}
};

CEREAL_REGISTER_TYPE(Item);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Item);