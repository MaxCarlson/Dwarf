#include "Gui.h"

#include "BearLibTerminal.h"

#include "Engine.h"
#include "Map.h"
#include "MapRender.h"

static const int GUI_PANEL_VERT_PIX = 4;


Gui::Gui()
{
}


Gui::~Gui()
{
}

void Gui::render()
{
	// Grab window size data
	panelWidth = terminal_state(TK_WIDTH);
	panelHeight = terminal_state(TK_HEIGHT);


	// Calculate panel size
	verticalOffset = panelHeight - GUI_PANEL_VERT_PIX;

	// Bottom gui
	terminal_clear_area(0, verticalOffset, panelWidth, panelHeight);

	drawGui();
}

void Gui::drawGui()
{
	// Gui outline
	terminal_color("light grey");

	int y = verticalOffset;

	// Draw horizontal border
	int horizCode = 0x2580;
	for (int i = 0; i < 2; ++i) {
		for (int w = 0; w < panelWidth; ++w)
			terminal_put(w, y, horizCode);

		y = panelHeight - 1;
		horizCode = 0x2583;
	}
	
	drawButtons();
}

void Gui::drawButtons()
{

	int buttonSize = panelWidth / numberOfButtons;

	int xButtonOffset = 0;

	static const char* guiMainColor = "light blue";
	static const char* guiButtonSepColor = "grey";
	static const char* buttonNames[numberOfButtons] = { "Orders", "Build", "Jobs", "Military", "Stockpile", "Stuff" };

	// Place button outlines
	for (int i = 0; i < numberOfButtons + 1; ++i)
	{
		// Store top left coordinate of buttons
		// so we can use them for finding clicks / hovers
		if(i < numberOfButtons)
			buttonCoordiantes[i] = { xButtonOffset, verticalOffset, 0 };

		for (int w = xButtonOffset; w < xButtonOffset + buttonSize; ++w)
		{
			for (int h = verticalOffset; h < panelHeight; ++h)
			{
				terminal_color(guiMainColor);
				terminal_put(w, h, 0x2588);

				if (w == xButtonOffset + buttonSize - 1 && i < numberOfButtons - 1) {
					terminal_color(guiButtonSepColor);
					terminal_put(w, h, 0x2502);
				}
			}
		}
		if (i < numberOfButtons)
			terminal_print(xButtonOffset + 1, verticalOffset + 2, buttonNames[i]);

		xButtonOffset += buttonSize;
	}
}

