#pragma once

struct Coordinates;

namespace mouse
{
	// Mouse position relative to tiles
	extern int mouseX;
	extern int mouseY;
	extern int mouseZ;

	extern Coordinates mousePos;

	// Mouse positon in pixels
	extern int mousePX;
	extern int mousePY;

	extern bool leftClick;
	extern bool rightClick;
	extern bool middleClick;
	extern bool wheelUp;
	extern bool wheelDown;


	void readMouse();
}

