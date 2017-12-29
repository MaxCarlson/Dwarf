#pragma once
#include "../Component.h"
#include "../cereal/types/utility.hpp"

struct Building : public Component
{
	Building() = default;
	Building(std::string tag, int width, int height, bool complete, const std::vector<int>& charCodes)
		: tag(tag), width(width), height(height), complete(complete), charCodes(charCodes) {}

	std::string tag;

	int width, height;

	bool complete = false;

	std::vector<int> charCodes;

	std::vector<std::pair<std::string, std::size_t>> materials;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(tag, width, height, complete, charCodes, materials);
	}
};

CEREAL_REGISTER_TYPE(Building);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Building);