#include "Gui.h"
#include "BearLibTerminal.h"
#include "Engine.h"
#include "Map/Map.h"
#include "Map/MapRender.h"
#include "Raws\Materials.h"
#include "Raws\Defs\MaterialDef.h"
#include "Raws\Buildings.h"
#include "Map\Tile.h"
#include "Raws\ItemRead.h"
#include "Drawing\draw.h"

using namespace region;

static const std::string gui_color = "#386687";

inline void wrapInput(int& n, int size)
{
	if (n < 0)
		n = size - 1;
	if (n > size - 1)
		n = 0;
}

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

	if (engine->current_game_state == Engine::PLAY)
	{
		switch (state)
		{
		case Gui_State::NO_DISPLAY:
			return;

		case Gui_State::ESC_MENU:
			break;

		case Gui_State::MAIN:
			drawMain();
			break;

		case Gui_State::BUILD:
			drawBuild();
			break;

		case Gui_State::DESIGNATE:
			drawDesignate();
			break;

		case Gui_State::ORDERS:
			break;

		case Gui_State::CREATE_ITEM:
			drawCreateItem();
			break;

		}

		printDebugTileProps();
	}
	
}

void Gui::clearAndDraw(int x, int y, int width, int height, const std::string color, int symbol)
{
	terminal_clear_area(x, y, width, height);
	terminal_color(color.c_str());

	for (int yy = y; yy < height; ++yy)
		for (int xx = x; xx < width; ++xx)
			terminal_put(xx, yy, symbol);
}

void Gui::drawMain()
{
	horizontalOffset = panelWidth - (panelWidth / 5);
	verticalOffset = 0;
	itemSelected = 0;

	clearAndDraw(horizontalOffset, verticalOffset, panelWidth, panelHeight, gui_color, 0x2588);

	static const std::vector<std::string> commands = { "b = build", "i = spawn item", "d = designate", "o = order" };

	int y = 2;
	for (const auto& c : commands)
	{
		terminal_color("black");
		terminal_print(horizontalOffset + 1, y, c.c_str());
		y += 2;
	}
}

void Gui::drawBuild()
{
	static const std::vector<std::string> buildings = get_all_building_def_names();

	clearAndDraw(horizontalOffset, verticalOffset, panelWidth, panelHeight, gui_color, 0x2588);
	wrapInput(itemSelected, buildings.size());

	int c = 0;
	int y = 2;
	for (const auto& b : buildings)
	{
		draw::determineHighlight(c, itemSelected);

		terminal_print(horizontalOffset + 1, y, b.c_str());
		y += 2;
		++c;
	}
	terminal_bkcolor("black");
}

void Gui::drawDesignate()
{
	static std::vector<std::string> dType = { "Mining", "Channeling" };

	clearAndDraw(horizontalOffset, verticalOffset, panelWidth, panelHeight, gui_color, 0x2588);
	wrapInput(itemSelected, dType.size());

	int y = 2;
	int c = 0;
	for (const auto& d : dType)
	{
		draw::determineHighlight(c, itemSelected);

		terminal_print(horizontalOffset + 1, y, d.c_str());
		y += 2;
		++c;
	}
	terminal_bkcolor("black");
}

void Gui::drawCreateItem()
{
	static auto itemTags = get_all_item_tags();

	clearAndDraw(horizontalOffset, verticalOffset, panelWidth, panelHeight, gui_color, 0x2588);

	wrapInput(itemSelected, itemTags.size());

	int y = 2;
	int c = 0;
	for (const auto& i : itemTags)
	{
		draw::determineHighlight(c, itemSelected);

		terminal_print(horizontalOffset + 1, y, i.c_str());
		y += 2;
		++c;
	}
	terminal_bkcolor("black");
}

void Gui::printDebugTileProps()
{
	int xx = terminal_state(TK_MOUSE_X);
	int yy = terminal_state(TK_MOUSE_Y);

	// debug print info on tile
	const int z = engine->mapRenderer->currentZLevel;

	auto matIdx = region::getTileMaterial({ xx, yy, z });

	auto mat = getMaterial(matIdx); 

	std::string tInfo = mat->name + '\n' + mat->layer + '\n';
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


	terminal_color("black");
	terminal_printf(xx, yy, tInfo.c_str());
}