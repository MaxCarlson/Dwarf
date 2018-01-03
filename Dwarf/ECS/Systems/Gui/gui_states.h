#pragma once

enum gui_states
{
	GUI_MAIN = 0,
	GUI_ESC_MENU,
	GUI_BUILD,
	GUI_ARCHITECTURE,
	GUI_DESIGNATE,
	GUI_ORDERS,
	GUI_CREATE_ITEM,
	GUI_REACTIONS,
	GUI_STOCKPILES
};

struct GuiState
{
	gui_states state = GUI_MAIN;

	int itemSelected = 0;

	bool typing = false;
	std::string inputText;
};


extern GuiState guiState;