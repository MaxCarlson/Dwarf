#pragma once

#include "../Systems.h"
#include "../../Coordinates.h"

struct entity_moved_message : public base_message_t
{
	entity_moved_message() = default;
	entity_moved_message(const Entity e, Coordinates newCo, Coordinates oldCo) : e(e), newCo(newCo), oldCo(oldCo) {}

	const Entity e;
	Coordinates newCo;
	Coordinates oldCo;
};