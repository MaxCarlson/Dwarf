#pragma once
#include "../Systems.h"


enum ArchitectureType
{
	WALL,
	FLOOR,
	RAMP,
	UP_STAIRS,
	DOWN_STAIRS,
	UP_DOWN_STAIRS,
	BRIDGE
};

struct designate_architecture_message : base_message_t
{
	designate_architecture_message() = default;
	designate_architecture_message(int type, std::pair<int, int> area) : type(type), area(area) {}

	// Should be on of the ArchitectureType's above
	int type;

	std::pair<int, int> area;
};