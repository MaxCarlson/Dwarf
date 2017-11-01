#include "Actor.h"
#include "Engine.h"
#include "Map.h"
#include "Ai.h"
#include "Attacker.h"
#include "Destructible.h"
#include "Pickable.h"
#include "Container.h"

#include <iostream>


Actor::Actor(Coordinates co, int ch, const char * name, const TCODColor & col)
	: co(co), ch(ch), col(col), blocks(true), attacker(NULL), destructible(NULL), ai(NULL), pickable(NULL), container(NULL)
{
	strcpy_s(this->name, name);	
}

Actor::~Actor()
{
	if (attacker)     delete attacker;
	if (destructible) delete destructible;
	if (ai)           delete ai;
	if (pickable)     delete pickable;
	if (container)    delete container;
}


void Actor::render() const {
	TCODConsole::root->setChar(co.x, co.y, ch);
	TCODConsole::root->setCharForeground(co.x, co.y, col);
}

void Actor::update()
{
	if (ai)
		ai->update(this);
}
