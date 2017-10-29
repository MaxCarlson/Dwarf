#include "Actor.h"
#include "Engine.h"
#include "Map.h"
#include <iostream>


Actor::Actor(int x, int y, int ch, const char * name, const TCODColor & col) : x(x), y(y), ch(ch), col(col)
{
	strcpy_s(this->name, name);
}


void Actor::render() const {
	TCODConsole::root->setChar(x, y, ch);
	TCODConsole::root->setCharForeground(x, y, col);
}

void Actor::update()
{
	std::cout << "The " << name << " growls..!" << std::endl;
}

bool Actor::moveOrAttack(int x, int y)
{
	if (engine.map->isWall(x, y))
		return false;

	for(Actor * actor : engine.actors)
		if (actor->x == x && actor->y == y) {
			printf("The %s laughs at your puny efforts to attack him!\n",
				actor->name);
			return false;
		}

	// Move character if not attacking/ wall
	this->x = x;
	this->y = y;
	return true;
}
