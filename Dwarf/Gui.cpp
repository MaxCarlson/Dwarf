#include "Gui.h"

#include "BearLibTerminal.h"

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
	// Also used in MapRender to calc offsets
	horizontalOffset = guiState;

	terminal_clear_area(panelWidth - horizontalOffset, 0, horizontalOffset, panelHeight);

	switch (guiState)
	{
	case SMALL:
		drawSmallGui();
		break;

	case LARGE:

		break;
	}


	// Loop through panels dedicated to Gui
	for(int w = panelWidth - guiState; w < panelWidth; ++w)
		for (int h = 0; h < panelHeight; ++h)
		{
			terminal_put(w, h, 0x2500);
		}
}

void Gui::drawSmallGui()
{
	// Draw gui outline
	int y = 0;
	terminal_color("light grey");

	int horizCode = 0x2580;
	// Draw horizontal border
	for (int i = 0; i < 2; ++i) {
		for (int w = panelWidth - guiState; w < panelWidth; ++w)
			terminal_put(w, y, horizCode);

		y = panelHeight - 1;
		horizCode = 0x2583;
	}

	int x = panelWidth - horizontalOffset;

	for (int i = 0; i < 2; ++i) {
		for (int h = 0; h < panelHeight; ++h)
			terminal_put(x, h, 0x2588);

		x = panelWidth - 1;
	}
	
}
