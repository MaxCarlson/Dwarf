#pragma once
#include "Coordinates.h"
#include <unordered_map>
#include "cereal\types\unordered_map.hpp"
#include <vector>
#include <memory>


struct Designations
{
	// Map of all areas designated to be mined. Bit's set for what type of mining
	// Indexed by tiles location in tileManager's vector of tiles
	std::unordered_map<int, uint8_t> mining;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(mining);
	}

};

extern std::unique_ptr<Designations> designations;