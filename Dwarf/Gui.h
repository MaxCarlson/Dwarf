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

	int verticalSize;
	int horizontalSize;

	// Array of button top left locations
	Coordinates buttonCoordiantes[numberOfButtons];

	bool isMouseOverButton(Coordinates co);
	void highlightButton(Coordinates co);

private:
	int panelWidth, panelHeight;

	void drawGui();
	void drawButtons();

	

	// Finds which button mouse is over
	// must call isMouseOverButton first to get
	// accurate result
	int whichButton(Coordinates co);
};

