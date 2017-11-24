#include "MainMenu.h"
#include "BearLibTerminal.h"
#include "Engine.h"
#include "EntityFactory.h"


static const int NUM_MAIN_MEN_OPTIONS = 4;

int MainMenu::render()
{
	panelWidth = terminal_state(TK_WIDTH);
	panelHeight = terminal_state(TK_HEIGHT);

	int selected = 0;

	while (true)
	{
		terminal_clear();

		determineHighlight(selected, START_GAME_O);
		terminal_print_ext(0, panelHeight / 2 - 15, panelWidth, panelHeight, TK_ALIGN_CENTER, "Start Game");

		determineHighlight(selected, CREATE_WORLD_O);
		terminal_print_ext(0, panelHeight / 2 - 10, panelWidth, panelHeight, TK_ALIGN_CENTER, "Create World");

		determineHighlight(selected, SETTINGS_O);
		terminal_print_ext(0, panelHeight / 2 - 5, panelWidth, panelHeight, TK_ALIGN_CENTER, "Settings");

		determineHighlight(selected, QUIT_O);
		terminal_print_ext(0, panelHeight / 2, panelWidth, panelHeight, TK_ALIGN_CENTER, "Quit");
		
		terminal_refresh();
		resetColor();

		selected = mainMenuInput(selected);

		if (selected == EXIT_CODE)
			return EXIT_CODE;

		else if (selected == START_CODE)
			return START_CODE;
	}
}

void MainMenu::setColor()
{
	terminal_bkcolor("#d1ce38");
	terminal_color("black");
}

void MainMenu::resetColor()
{
	terminal_color("default");
	terminal_bkcolor("black");
}

void MainMenu::determineHighlight(int h, int num)
{
	if (num == h)
		setColor();
	else
		resetColor();
}

int MainMenu::mainMenuInput(int selected)
{
	const int key = terminal_read();
	
	switch (key)
	{
	case TK_DOWN:
		++selected;
		break;

	case TK_UP:
		--selected;
		break;

	case TK_ENTER:
		switch (selected)
		{
		case START_GAME_O:
			return startGame();
			break;

		case CREATE_WORLD_O:
			break;

		case SETTINGS_O:
			//settings();
			break;

		case QUIT_O:
			return EXIT_CODE;
		}

		break;
	}


	if (selected < 0)
		selected = NUM_MAIN_MEN_OPTIONS - 1;

	else if (selected >= NUM_MAIN_MEN_OPTIONS)
		selected = 0;

	return selected;
}

// Eventually this function will handle
// picking an already generated world
// and loading info Right now it just tells us to pick
// dwaves
int MainMenu::startGame()
{
	if (pickDwarves())
		return START_CODE;

	return 0;
}

bool MainMenu::pickDwarves()
{
	int selected = 0;
	static const int NumberOfDwaves = 7;

	while (true)
	{
		terminal_clear();

		terminal_print_ext(0, 0, panelWidth, panelHeight, TK_ALIGN_LEFT, "Dwaves");

		int tileIndent = 2;
		for (int d = 0; d < NumberOfDwaves; ++d)
		{
			determineHighlight(selected, d);

			std::string ds = "Dwarf ";
			ds += d + '0';

			terminal_print_ext(0, tileIndent, panelWidth, panelHeight, TK_ALIGN_LEFT, ds.c_str());

			tileIndent += 1;
		}

		terminal_refresh();

		selected = pickDwarvesInput(selected, NumberOfDwaves);

		if (selected == EXIT_CODE)
			return false;
	}
}

int MainMenu::pickDwarvesInput(int selected, int maxNumber)
{
	const int key = terminal_read();

	switch (key)
	{
	case TK_ESCAPE:
		return EXIT_CODE;

	case TK_UP:
		--selected;
		break;

	case TK_DOWN:
		++selected;
		break;
	}

	if (selected >= maxNumber)
		selected = 0;
	else if (selected < 0)
		selected = maxNumber - 1;

	return selected;
}
