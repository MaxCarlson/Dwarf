#pragma once
#include "Map/Tile.h"

static const int numberOfButtons = 6;


class Gui
{
public:
	Gui();
	~Gui();

	// Render local map
	void render();

	// The starting locations of the 
	// Gui, top left coordinates
	int horizontalOffset;
	int verticalOffset;

	enum Gui_State
	{
		NO_DISPLAY,
		ESC_MENU,
		MAIN,
		BUILD,
		ORDERS
	};

	Gui_State state = MAIN;

private:
	int panelWidth, panelHeight;

	void drawMain();
	void drawBuild();

	void clearAndDraw(int x, int y, int width, int height, const std::string color, int symbol);

	void printDebugTileProps();
};

