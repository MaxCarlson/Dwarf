#pragma once
#include "include\libtcod.hpp"

static const int MAX_NAME_LENGTH = 20;

class Actor;
class Attacker;
class Destructible;
class Ai;
class Pickable;
class Container;

class Actor
{
public:
	Actor(int x, int y, int ch, const char * name, const TCODColor & col);
	~Actor();

	void render() const;
	void update();

	int x, y; 
	int ch;
	char name[MAX_NAME_LENGTH];  // Change to std::string eventually and and inline std::string to const char * method for using libtcod funcitons
	TCODColor col;
	bool blocks;				 // Does block movement?
	Attacker *attacker;			 // Can actor attack?
	Destructible * destructible; // Can actor be destroyed?
	Ai *ai;						 // actor has ai
	Pickable * pickable;		 // Can we pick the actor/object up? // Add subset to class wearable? Object can be worn after being picked up?
	Container * container;       // Can actor/object store things?
};

