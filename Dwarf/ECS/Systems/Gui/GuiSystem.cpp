#include "stdafx.h"
#include "GuiSystem.h"
#include "../../../Engine.h"
#include "../../../Map/Map.h"
#include "../../../Map/MapRender.h"
#include "../../../Raws\Materials.h"
#include "../../../Raws\Defs\MaterialDef.h"
#include "../../../Raws\Buildings.h"
#include "../../../Map\Tile.h"
#include "../../../Raws\ItemRead.h"
#include "../../../Drawing\draw.h"
#include "../../../Raws\ReadReactions.h"
#include "../../../Designations.h"
#include "../../../Raws\DefInfo.h"
#include "gui_states.h"
#include <filesystem>
using namespace region;
namespace fs = std::experimental::filesystem;

static const std::string gui_color = "#386687";

inline void wrapInput(int& n, std::size_t size)
{
	if (n < 0)
		n = size - 1;
	if (n > size - 1)
		n = 0;
}

void GuiSystem::render()
{
	// Grab window size data
	panelWidth = terminal_state(TK_WIDTH);
	panelHeight = terminal_state(TK_HEIGHT);

	horizontalOffset = panelWidth - (panelWidth / 5);
	verticalOffset = 0;

	if (engine->current_game_state == Engine::PLAY)
	{
		switch (guiState.state)
		{
		case gui_states::GUI_ESC_MENU:
			drawEscMenu();
			break;

		case gui_states::GUI_MAIN:
			drawMain();
			break;

		case gui_states::GUI_BUILD:
			drawBuild();
			break;

		case gui_states::GUI_ARCHITECTURE:
			drawArchitecture();
			break;

		case gui_states::GUI_DESIGNATE:
			drawDesignate();
			break;

		case gui_states::GUI_ORDERS:
			break;

		case gui_states::GUI_CREATE_ITEM:
			drawCreateItem();
			break;

		case gui_states::GUI_REACTIONS:
			drawReactions();
			break;

		case gui_states::GUI_STOCKPILES:
			drawStockpiles();
			break;

		}

		printDebugTileProps();
	}
}

void GuiSystem::clearAndDraw(int x, int y, int width, int height, const std::string color, int symbol)
{
	terminal_clear_area(x, y, width, height);
	terminal_color(color.c_str());

	for (int yy = y; yy < height; ++yy)
		for (int xx = x; xx < width; ++xx)
			terminal_put(xx, yy, symbol);
}

void GuiSystem::drawEscMenu()
{
	static const std::vector<std::string> escOptions = { "Return to game", "Save Game", "Quit To Main Menu" }; //
	int selected = 0;

	while (true)
	{
		int opt = draw::listHandler<std::string, true>(escOptions, selected, TK_ALIGN_CENTER, 0, 0, 4, true);

		if (opt == draw::IN_ENTER)
		{
			if (selected == 0)
				break;
			// Save game
			else if (selected == 1)
			{
				drawSaveScreen();
			}
			else if (selected == 2)
			{
				engine->current_game_state = Engine::TO_MAIN_MENU;
				break;
			}
		}
	}
	guiState.state = gui_states::GUI_MAIN;
}

void GuiSystem::drawMain()
{
	guiState.itemSelected = 0;

	clearAndDraw(horizontalOffset, verticalOffset, panelWidth, panelHeight, gui_color, 0x2588);

	static const std::vector<std::string> commands = { "b = build", "i = spawn item", "d = designate", "o = order", "m - reactions", "p - stockpiles", "t - architecture" };

	terminal_color("black");

	int y = 2;
	for (const auto& c : commands)
	{
		terminal_print(horizontalOffset + 1, y, c.c_str());
		y += 2;
	}
}

void GuiSystem::drawBuild()
{
	clearAndDraw(horizontalOffset, verticalOffset, panelWidth, panelHeight, gui_color, 0x2588);
	wrapInput(guiState.itemSelected, defInfo->buildingNames.size());

	int c = 0;
	int y = 2;
	for (const auto& b : defInfo->buildingNames)
	{
		draw::determineHighlight(c, guiState.itemSelected);

		terminal_print(horizontalOffset + 1, y, b.c_str());
		y += 2;
		++c;
	}
	terminal_bkcolor("black");
}

void GuiSystem::drawDesignate()
{
	static std::vector<std::string> dType = { "Mining", "Channeling" };

	clearAndDraw(horizontalOffset, verticalOffset, panelWidth, panelHeight, gui_color, 0x2588);
	wrapInput(guiState.itemSelected, dType.size());

	int y = 2;
	int c = 0;
	for (const auto& d : dType)
	{
		draw::determineHighlight(c, guiState.itemSelected);

		terminal_print(horizontalOffset + 1, y, d.c_str());
		y += 2;
		++c;
	}
	terminal_bkcolor("black");
}

void GuiSystem::drawCreateItem()
{
	clearAndDraw(horizontalOffset, verticalOffset, panelWidth, panelHeight, gui_color, 0x2588);

	wrapInput(guiState.itemSelected, defInfo->itemTags.size());

	int y = 2;
	int c = 0;
	for (const auto& i : defInfo->itemTags)
	{
		draw::determineHighlight(c, guiState.itemSelected);

		terminal_print(horizontalOffset + 1, y, i.c_str());
		y += 2;
		++c;
	}
	terminal_bkcolor("black");
}

void GuiSystem::drawReactions()
{
	clearAndDraw(horizontalOffset, verticalOffset, panelWidth, panelHeight, gui_color, 0x2588);

	wrapInput(guiState.itemSelected, defInfo->availibleReactions.size());

	int y = 2;
	int c = 0;
	terminal_print(horizontalOffset + 1, y, "Search for reaction");
	y = 4;

	// Add in searching for reactions by name + or by building 
	static std::vector<std::string> filteredReactions;

	// If we're not searching for reaction by text
	if (!guiState.typing)
	{
		if (defInfo->availibleReactions.size() == 0)
		{
			terminal_print(horizontalOffset + 1, y, "No reactions availible");
			terminal_print(horizontalOffset + 1, y + 2, "Try building workshops");
		}
		else
			for (const auto& p : defInfo->availibleReactions)
			{
				draw::determineHighlight(c, guiState.itemSelected);

				terminal_print(horizontalOffset + 1, y, p.c_str());
				y += 2;
				++c;
			}
	}
	else
	{
		filteredReactions.clear();
	}


}

void GuiSystem::drawStockpiles()
{
	clearAndDraw(horizontalOffset, verticalOffset, panelWidth, panelHeight, gui_color, 0x2588);

	wrapInput(guiState.itemSelected, defInfo->stockpileNames.size());

	int y = 2;
	int c = 0;
	for (const auto& i : defInfo->stockpileNames)
	{
		draw::determineHighlight(c, guiState.itemSelected);

		terminal_print(horizontalOffset + 1, y, i.c_str());
		y += 2;
		++c;
	}
	terminal_bkcolor("black");
}

void GuiSystem::drawArchitecture()
{
	static const std::vector<std::string> archNames = { "Wall", "Ramp", "Up Stairs", "Down Stairs", "Up Down Stairs", "Bridge" };

	wrapInput(guiState.itemSelected, archNames.size());

	int y = 2;
	int c = 0;
	for (const auto& i : archNames)
	{
		draw::determineHighlight(c, guiState.itemSelected);

		terminal_print(horizontalOffset + 1, y, i.c_str());
		y += 2;
		++c;
	}
	terminal_bkcolor("black");
}

void GuiSystem::printDebugTileProps()
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

void GuiSystem::drawSaveScreen()
{
	std::string fileName;

	std::string dirpath = "Saves";
	std::vector<std::string> paths = { "New save game" };
	for (auto & p : fs::directory_iterator(dirpath))
	{
		paths.push_back(p.path().string());
		//terminal_print_ext(10, 10, panelWidth, panelHeight, TK_ALIGN_LEFT, p.path().c_str());
	}

	if (!paths.size())
		paths.push_back("No save games on File! Press Esc");

	int selected = 0, code;

	while (true)
	{
		while (true)
		{
			code = draw::listHandler<std::string, true>(paths, selected, TK_ALIGN_LEFT, 7, 7, 2, true);

			if (code == draw::IN_EXIT)
				return;
			else if (code == draw::IN_ENTER)
				break;
		}

		// If enter was pressed on new save game
		if (code == draw::IN_ENTER && selected == 0)
		{
			paths[0] = "";

			std::wstring s;
			while (true)
			{
				// Draw list but skip input
				draw::listHandler<std::string, false>(paths, selected, TK_ALIGN_LEFT, 7, 7, 2, true, true, true);

				int input = terminal_read();

				if (input == TK_ESCAPE || (input == TK_ENTER && s.size() <= 0))
				{
					paths[0] = "No save games on File! Press Esc";
					break;
				}

				else if (input == TK_ENTER) // && s.size() > 0 ~ implied
					goto SAVE_REGION;

				// Delete chars
				else if (input == TK_BACKSPACE && s.length() > 0)
					s.resize(s.length() - 1);

				// Add chars
				else if (terminal_check(TK_WCHAR))
					s += (wchar_t)terminal_state(TK_WCHAR);

				paths[0] = { s.begin(), s.end() };
			}
		}
	}
SAVE_REGION:


	engine->saveGame(paths[0]);
}
