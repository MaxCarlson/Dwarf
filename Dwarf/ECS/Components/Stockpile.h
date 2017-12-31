#pragma once
#include "../Component.h"

struct Stockpile : public Component
{
	Stockpile() = default;
	Stockpile(const std::bitset<64> catagory) : catagory(catagory) {}

	std::bitset<64> catagory;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(catagory);
	}
};

CEREAL_REGISTER_TYPE(Stockpile);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Stockpile);
