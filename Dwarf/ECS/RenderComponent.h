#pragma once
#include "PositionComponent.h"
#include "../include/libtcod.hpp"

// Must be attached to an entity with a position
class RenderComponent : public Component
{
private:
	// What level is camera looking at, and by virtue of system
	// what level do we need to render?
	PositionComponent * position;
	int ch;
	bool activeDraw;
	// Background color
	TCODColor backColor;
	TCODColor foreColor;
public:
	RenderComponent() = default;
	RenderComponent(int ch = NULL) {};

	void init() override
	{
		position = &entity->getComponent<PositionComponent>();
	}
	void draw() override
	{
		if (activeDraw) {
			TCODConsole::root->setCharBackground(position->x(), position->y(), backColor);
			TCODConsole::root->setCharForeground(position->x(), position->y(), foreColor);
			if(ch)
				TCODConsole::root->setChar(position->x(), position->y(), ch);
		}
	}
};