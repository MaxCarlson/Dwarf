#pragma once
#include "../Component.h"

struct Stockpile : public Component
{

	std::bitset<64> catagory;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(catagory);
	}
};

CEREAL_REGISTER_TYPE(Stockpile);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Stockpile);
