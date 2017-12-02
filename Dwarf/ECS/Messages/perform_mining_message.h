#pragma once
#include "../Systems.h"

struct perform_mining_message : public base_message_t
{
	perform_mining_message() {}
	perform_mining_message(Entity ent, const int idx, const uint8_t opp) 
		                     : e(ent), targetIdx(idx), opperation(opp) {}

	Entity e;
	int targetIdx;
	uint8_t opperation;
};