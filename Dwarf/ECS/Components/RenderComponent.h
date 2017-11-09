#pragma once
#include "PositionComponent.h"

#include "../../include/libtcod.hpp"


// Holds info about character to use in render
// as well as colors. 
class RenderComponent : public Component
{
public:
	// Character code representation
	int ch;
	//bool activeDraw; // Should this be used for whether rock has been exposed/explored as a flag??

	// Colors for rendering
	// Find a way to make using one optional? 
	// is there a way not to set fore color for example?
	TCODColor backColor;
	TCODColor foreColor;

	RenderComponent() = default;
	RenderComponent(int ch, TCODColor backColor, TCODColor foreColor) 
		         : ch(ch), backColor(backColor), foreColor(foreColor) {};
};