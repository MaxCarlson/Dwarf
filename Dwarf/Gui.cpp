#include "Gui.h"

#include "BearLibTerminal.h"

#include "Engine.h"
#include "Map/Map.h"
#include "Map/MapRender.h"

static const int GUI_PANEL_VERT_PIX = 4;
static const char* guiMainColor = "#386687";
static const char* guiHighlightColor = "#4a87b2";
static const char* guiButtonSepColor = "grey";
static const char* buttonNames[numberOfButtons] = { "Orders", "Build", "Jobs", "Military", "Stockpile", "Stuff" };

Gui::Gui() : horizontalOffset(0), verticalOffset(0), horizontalSize(0), verticalSize(GUI_PANEL_VERT_PIX)
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

	if (engine->current_game_state == Engine::PLAY)
	{
		// Calculate panel size
		verticalOffset = panelHeight - GUI_PANEL_VERT_PIX;

		// Bottom gui
		terminal_clear_area(0, verticalOffset, panelWidth, panelHeight);

		drawGui();
	}

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

	printDebugTileProps();
}

void Gui::drawButtons()
{

	int buttonSize = panelWidth / numberOfButtons;

	int xButtonOffset = 0;

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

bool Gui::isMouseOverButton(Coordinates co)
{
	return co.y >= buttonCoordiantes[0].y;
}

int Gui::whichButton(Coordinates co)
{
	for (int i = 0; i < numberOfButtons - 1; ++i)
	{
		if (co.x < buttonCoordiantes[i].x)
			return i;
	}
	return numberOfButtons - 1;
}

void Gui::highlightButton(Coordinates co)
{
	int bttnIdx = whichButton(co);

	int x = buttonCoordiantes[bttnIdx].x;
	int xStop;

	if (bttnIdx == numberOfButtons - 1)
		xStop = panelWidth;
	else
		xStop = buttonCoordiantes[bttnIdx + 1].x;

	int y = buttonCoordiantes[bttnIdx].y;
	int yStop = panelHeight;



	for(x; x < xStop; ++x)
		for (y; y < yStop; ++y)
		{
			terminal_color("black");
			terminal_put(x, y, 0x2588);
		}
}

#include "Map\Tile.h"
using namespace region;

void Gui::printDebugTileProps()
{
	terminal_color("red");

	int xx = terminal_state(TK_MOUSE_X);
	int yy = terminal_state(TK_MOUSE_Y);

	// debug print info on tile
	const int z = engine->map->mapRenderer->currentZLevel;

	std::string tInfo = "DEFAULT \n";
	bool csth = flag({ xx, yy, z }, CAN_STAND_HERE);

	if (csth)
		tInfo += "CAN_STAND_HERE \n";

	csth = flag({ xx, yy, z }, CAN_GO_NORTH);

	if (csth)
		tInfo += "CAN_GO_NORTH \n";

	csth = flag({ xx, yy, z }, CAN_GO_SOUTH);

	if (csth)
		tInfo += "CAN_GO_SOUTH \n";

	csth = flag({ xx, yy, z }, CAN_GO_EAST);

	if (csth)
		tInfo += "CAN_GO_EAST \n";

	csth = flag({ xx, yy, z }, CAN_GO_WEST);

	if (csth)
		tInfo += "CAN_GO_WEST \n";

	csth = flag({ xx, yy, z }, CAN_GO_UP);

	if (csth)
		tInfo += "CAN_GO_UP \n";

	csth = flag({ xx, yy, z }, CAN_GO_DOWN);

	if (csth)
		tInfo += "CAN_GO_DOWN \n";

	terminal_printf(xx, yy, tInfo.c_str());
}