#pragma once
class Gui
{
public:
	Gui();
	~Gui();

	void render();

	int horizontalOffset;
	int verticalOffset;

	enum GuiState
	{
		// Size in tiles of gui
		OFF = 0,
		SMALL = 20,
		LARGE = 27
	} guiState = SMALL;

private:
	int panelWidth, panelHeight;

	void drawSmallGui();
};

