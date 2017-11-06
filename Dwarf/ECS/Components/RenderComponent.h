#pragma once
#include "PositionComponent.h"

#include "../../include/libtcod.hpp"


// Holds info about character to use in render
// as well as colors. 
class RenderComponent : public Component
{
public:
	// Character representation
	int ch;
	//bool activeDraw; // This shouldn't be required, as we can just de-activate the render component for render system

	// Colors for rendering
	TCODColor backColor;
	TCODColor foreColor;

	RenderComponent() = default;
	RenderComponent(int ch, TCODColor backColor, TCODColor foreColor) 
		         : ch(ch), backColor(backColor), foreColor(foreColor) {};
};