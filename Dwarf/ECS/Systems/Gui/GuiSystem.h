#pragma once
#include "../../Systems.h"


class GuiSystem : public System<Requires<>>
{
public:
	// Render local map
	void render();

	// The starting locations of the 
	// Gui, top left coordinates
	int horizontalOffset;
	int verticalOffset;

private:
	int panelWidth, panelHeight;

	void clearAndDraw(int x, int y, int width, int height, const std::string color, int symbol);

	// Move most of these functions into sepperate files
	// once they become more complex
	void drawEscMenu();
	void drawMain();
	void drawBuild();
	void drawDesignate();
	void drawCreateItem();
	void drawReactions();
	void drawStockpiles();
	void drawArchitecture();
	void printDebugTileProps();
	void drawSaveScreen();
};


