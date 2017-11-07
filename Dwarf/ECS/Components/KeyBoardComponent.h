#pragma once

#include "../Component.h"
#include "../../include/libtcod.hpp"

// Must be assigned the TCOD_key after being assigned to entity
class KeyboardComponent : public Component
{
public:
	KeyboardComponent() = default;
	//KeyboardComponent(TCOD_key_t* lastKeyPressed) : lastKeyPressed(lastKeyPressed) {}; /// Doesn't work?

	TCOD_key_t* lastKeyPressed;
};