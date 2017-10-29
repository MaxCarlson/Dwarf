#pragma once
#include "include\libtcod.hpp"

static const int MAX_NAME_LENGTH = 20;

class Actor;
class Attacker;
class Destructible;
class Ai;

class Actor
{
public:
	Actor(int x, int y, int ch, const char * name, const TCODColor & col);

	void render() const;
	void update();

	int x, y; 
	int ch;
	char name[MAX_NAME_LENGTH];
	TCODColor col;
	bool blocks; // Does block movement?
	Attacker *attacker; // Can actor attack?
	Destructible * destructible; // Can actor be destroyed?
	Ai *ai;						// actor has ai
};

