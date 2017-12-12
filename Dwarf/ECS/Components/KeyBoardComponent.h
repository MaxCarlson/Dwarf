#pragma once

#include "../Component.h"
#include "../../include/libtcod.hpp"

// Must be assigned the TCOD_key after being assigned to entity /////////// No longer need last key?
class KeyboardComponent : public Component
{
public:
	KeyboardComponent() = default;
	template<class Archive>
	void serialize(Archive& archive)
	{

	}
};