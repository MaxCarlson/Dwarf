#pragma once
#include "include\libtcod.hpp"

static const int MAX_NAME_LENGTH = 20;

class Actor
{
public:
	Actor(int x, int y, int ch, const char * name, const TCODColor & col);

	void render() const;
	void update();
	bool moveOrAttack(int x, int y);

	int x, y; 
	int ch;
	char name[MAX_NAME_LENGTH];
	TCODColor col;
};

