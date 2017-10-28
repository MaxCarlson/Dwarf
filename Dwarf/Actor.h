#pragma once
#include "include\libtcod.hpp"

class Actor
{
public:
	Actor(int x, int y, int ch, const TCODColor & col);

	void render() const;

	int x, y; // pos
	int ch;
	TCODColor col;
};

