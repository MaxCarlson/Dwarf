#pragma once

#include "../Component.h"

class Item : public Component
{
public:
	Item() = default;
	//Item(const std::string name) : name(name) {}
	Item(std::string& name, std::string& tag, std::bitset<64>& catagory, std::size_t material, int stackSize) 
		: name(name), tag(tag), catagory(catagory), material(material), stackSize(stackSize) {}

	std::string name;
	std::string tag;
	std::bitset<64> catagory;
	std::size_t material;
	int stackSize = 1;

	bool claimed = false;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::base_class<Component>(this), name, tag, catagory, material, stackSize, claimed);
	}
};

CEREAL_REGISTER_TYPE(Item);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Item);