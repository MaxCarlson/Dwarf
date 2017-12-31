#pragma once
#include "../Systems.h"

struct Coordinates;

struct request_new_stockpile_message : public base_message_t
{
	request_new_stockpile_message() = default;
	request_new_stockpile_message(std::pair<int, int> area, std::bitset<64> setCatagories) : area(area), setCatagories(setCatagories) {}

	std::pair<int, int> area;

	std::bitset<64> setCatagories;
};