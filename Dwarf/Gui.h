#pragma once
#include "Tile.h"

static const int numberOfButtons = 6;


class Gui
{
public:
	Gui();
	~Gui();

	void render();

	int horizontalOffset;
	int verticalOffset;

	// Array of button top left locations
	Coordinates buttonCoordiantes[numberOfButtons];

private:
	int panelWidth, panelHeight;

	void drawGui();
	void drawButtons();
};

