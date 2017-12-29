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
		DESIGNATE,
		ORDERS,
		CREATE_ITEM,
		REACTIONS
	};

	Gui_State state = MAIN;

	int itemSelected = 0;

private:
	int panelWidth, panelHeight;

	void clearAndDraw(int x, int y, int width, int height, const std::string color, int symbol);

	void drawEscMenu();
	void drawMain();
	void drawBuild();
	void drawDesignate();
	void drawCreateItem();
	void drawReactions();
	void printDebugTileProps();
	void drawSaveScreen();
};

