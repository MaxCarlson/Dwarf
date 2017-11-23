#include "MainMenu.h"
#include "BearLibTerminal.h"



static const int NUM_MAIN_MEN_OPTIONS = 4;

int MainMenu::render()
{
	int panelWidth = terminal_state(TK_WIDTH);
	int panelHeight = terminal_state(TK_HEIGHT);

	int selected = 0;

	while (true)
	{
		terminal_clear();

		determineHighlight(selected, START_GAME);
		terminal_print_ext(0, panelHeight / 2 - 15, panelWidth, panelHeight, TK_ALIGN_CENTER, "Start Game");

		determineHighlight(selected, CREATE_WORLD);
		terminal_print_ext(0, panelHeight / 2 - 10, panelWidth, panelHeight, TK_ALIGN_CENTER, "Create World");

		determineHighlight(selected, SETTINGS);
		terminal_print_ext(0, panelHeight / 2 - 5, panelWidth, panelHeight, TK_ALIGN_CENTER, "Settings");

		determineHighlight(selected, QUIT);
		terminal_print_ext(0, panelHeight / 2, panelWidth, panelHeight, TK_ALIGN_CENTER, "Quit");
		
		terminal_refresh();
		resetColor();

		selected = mainMenuInput(selected);

		if (selected == EXIT_CODE)
			return EXIT_CODE;
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
		case START_GAME:
			break;

		case CREATE_WORLD:
			break;

		case SETTINGS:
			break;

		case QUIT:
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
