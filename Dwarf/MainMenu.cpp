#include "MainMenu.h"
#include "BearLibTerminal.h"


MainMenu::MainMenu()
{
}


MainMenu::~MainMenu()
{
}

static const int NUM_MAIN_MEN_OPTIONS = 3;

void MainMenu::render()
{
	int panelWidth = terminal_state(TK_WIDTH);
	int panelHeight = terminal_state(TK_HEIGHT);

	int selected = 0;

	while (true)
	{
		terminal_clear();

		determineHighlight(selected, 0);
		terminal_print_ext(0, panelHeight / 2 - 15, panelWidth, panelHeight, TK_ALIGN_CENTER, "Start Game");

		determineHighlight(selected, 1);
		terminal_print_ext(0, panelHeight / 2 - 10, panelWidth, panelHeight, TK_ALIGN_CENTER, "Settings");

		determineHighlight(selected, 2);
		terminal_print_ext(0, panelHeight / 2 - 5, panelWidth, panelHeight, TK_ALIGN_CENTER, "Quit");

		terminal_refresh();

		selected = processInput(selected);
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

int MainMenu::processInput(int selected)
{
	int key = terminal_read();

	if (key == TK_DOWN)
		
	
	switch (key)
	{
	case TK_DOWN:
		++selected;
		break;

	case TK_UP:
		--selected;
		break;
	}


	if (selected < 0)
		selected = NUM_MAIN_MEN_OPTIONS - 1;

	else if (selected >= NUM_MAIN_MEN_OPTIONS)
		selected = 0;

	return selected;
}
