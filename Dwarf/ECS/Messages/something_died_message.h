#pragma once
#include "ECS\Systems.h"

struct something_died_message : public base_message_t
{
	something_died_message() = default;
	something_died_message(size_t id, size_t killer) : id(id), killer(killer) {}

	size_t id = 0;
	size_t killer = 0;
};