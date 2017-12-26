#pragma once
#include "../Systems.h"
#include "../Raws/Defs/BuildingDef.h"
#include "../Coordinates.h"

struct designate_building_message : public base_message_t
{
	designate_building_message() = default;
	designate_building_message(BuildingDef building, int idx) : building(building), idx(idx) {}

	BuildingDef building;

	// Top left building location
	int idx;
};