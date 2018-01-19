#pragma once
#include "Coordinates.h"
#include "ECS\Components\helpers\building_designation.h"
#include <unordered_map>
#include "cereal\types\unordered_map.hpp"
#include "cereal\types\utility.hpp"
#include <vector>
#include <memory>


struct Designations
{
	// Map of all areas designated to be mined. Bit's set for what type of mining
	// Indexed by tiles location 
	std::unordered_map<int, uint8_t> mining;

	// Holds all the Coordinates of tree's designated for chopping
	// Indexed by tree ID
	std::unordered_map<int, Coordinates> woodcutting;

	// Buildings waiting to be built
	std::vector<building_designation> buildings;

	// Map of designated architecture locations, indexed by vector idx
	// pair.first is architecture type, pair.second is % completion 
	std::unordered_map<int, std::pair<uint8_t, double>> architecture;

	// Work orders that cannot be completed yet due to materials, not enough workshopds, etc
	// pair<qty, reactionName>
	std::vector<std::pair<int, std::string>> queuedWorkOrders;

	// Work orders that can be completed right now
	// pair<qty, reactionName>
	std::vector<std::pair<int, std::string>> workOrders;

	// Map of items indexed by destination square that are currently claimed
	// and or being taken to a stockpile.
	// size_t is the stockpile id
	std::unordered_map<int, std::size_t> hauling;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(mining, buildings, architecture, workOrders, hauling);
	}

};

extern std::unique_ptr<Designations> designations;