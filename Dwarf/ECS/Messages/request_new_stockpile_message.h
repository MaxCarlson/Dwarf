#pragma once
#include "../Systems.h"

struct Coordinates;

struct request_new_stockpile_message : public base_message_t
{
	request_new_stockpile_message() = default;
	request_new_stockpile_message(std::pair<Coordinates, Coordinates> area, int type) : area(area), type(type) {}

	std::pair<Coordinates, Coordinates> area;

	int type = 0;
};