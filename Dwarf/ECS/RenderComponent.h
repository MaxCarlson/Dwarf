#pragma once
#include "PositionComponent.h"
#include "Camera.h"
#include "../include/libtcod.hpp"

/*
// Must be attached to an entity with a position
// If you ever want it to be rendered, you need to attach
// the camera component of the master camera to it so we can grab z Level
class RenderComponent : public Component
{
private:
	// What level is camera looking at, and by virtue of system
	// what level do we need to render?
	PositionComponent * position;

	// This is a pointer to the master camera component
	// from the master camera entity. Should allow us to
	// Only render the zLvl the camera is at
	CameraComponent * masterCamera;
	int ch;

	// Flag set to denote if we want to currently render this Entity
	bool activeDraw = false;
	// Background color
	TCODColor backColor;
	TCODColor foreColor;
public:
	RenderComponent() = default;
	RenderComponent(CameraComponent * masterCamera, int ch, bool activeDraw, TCODColor backColor, TCODColor foreColor) 
		               : masterCamera(masterCamera), ch(ch), activeDraw(activeDraw), backColor(backColor), foreColor(foreColor) {};

	void init() override
	{
		position = &entity->getComponent<PositionComponent>();
	}
	void draw() override
	{
		// Only render if the camera is on our current z Level
		if (activeDraw && masterCamera->currentZLevel() == position->z()) {
			TCODConsole::root->setCharBackground(position->x(), position->y(), backColor);
			TCODConsole::root->setCharForeground(position->x(), position->y(), foreColor);
			if(ch)
				TCODConsole::root->setChar(position->x(), position->y(), ch);
		}
	}
};
*/