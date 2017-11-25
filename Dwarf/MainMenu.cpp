#include "MainMenu.h"
#include "BearLibTerminal.h"
#include "Engine.h"
#include "EntityFactory.h"
#include "ECS\Components\JobComponent.h"


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
// Make all these input functions into one big template?
int MainMenu::mainMenuInput(int selected)
{
	const int key = terminal_read();

	upOrDownInput(key, selected);
	
	switch (key)
	{
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

	// Wrap scrolling highlighting
	if (selected < 0)
		selected = NUM_MAIN_MEN_OPTIONS - 1;

	else if (selected >= NUM_MAIN_MEN_OPTIONS)
		selected = 0;

	return selected;
}

void MainMenu::upOrDownInput(int key, int & selected)
{
	switch (key)
	{
	case TK_DOWN:
		++selected;
		break;

	case TK_UP:
		--selected;
		break;

	default:
		break;
	}
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

	dwarfStats.resize(NumberOfDwaves);

	// Do we want to be scrolling through
	// stats or dwaves?
	bool statsSelected = false;

	while (true)
	{
		terminal_clear();

		terminal_print_ext(0, 0, panelWidth, panelHeight, TK_ALIGN_LEFT, "Dwaves");

		// Print out dwarfs to select from
		// Generate names/allow name picking eventually
		int tileIndent = 2;
		for (int d = 0; d < NumberOfDwaves; ++d)
		{
			determineHighlight(selected, d);

			// Rudimentry naming
			std::string ds = "Dwarf ";
			ds += d + 1 + '0';

			terminal_print_ext(0, tileIndent, panelWidth, panelHeight, TK_ALIGN_LEFT, ds.c_str());
			
			tileIndent += 1;
		}

		printDwafStatOptions(selected, statsSelected);

		terminal_refresh();
		resetColor();

		selected = pickDwarvesInput(selected, statsSelected, NumberOfDwaves);

		// Local exit of this menu to main menu
		if (selected == EXIT_CODE)
			return false;
	}
}

int MainMenu::pickDwarvesInput(int selected, bool &statsSelected, int maxNumber)
{
	const int key = terminal_read();

	upOrDownInput(key, selected);

	switch (key)
	{
	case TK_ESCAPE:
		return EXIT_CODE;

	case TK_ENTER: // Fall Through..
	case TK_RIGHT:
		statsSelected = true;
		break;
	}

	// Wrap scrolling highlighting
	if (selected >= maxNumber)
		selected = 0;
	else if (selected < 0)
		selected = maxNumber - 1;

	return selected;
}

void MainMenu::printDwafStatOptions(int dwarfSelected, bool &statsSelected)
{
	int statSelected = 0;
	static const int statIndent = 15;
	static const int statValueIndent = 25;

	static const int totalStatNumber = listOfAllJobsByIndex.size(); // + listOfAllCombatStats.Size()

	// Resize each dwarves stat vector
	{
		int statCounterDwarf = 0;
		for (auto& dwarVec : dwarfStats)
			dwarfStats[statCounterDwarf++].resize(totalStatNumber);
	}
		
	do
	{
		terminal_clear_area(12, 0, panelWidth, panelHeight);

		// Labor Stats
		int tileIndent = 2;
		int statCounter = -1; // -1 due to first job being Job::NONE
		for (int stat : listOfAllJobsByIndex)
		{
			if (stat == Job::Jobs::NONE)
				continue;

			if(statsSelected)
				determineHighlight(statSelected, stat - 1);

			const char * jobStr = listOfAllJobsByString[stat].c_str();

			// Print the stat
			terminal_print_ext(statIndent, tileIndent, panelWidth, panelHeight, TK_ALIGN_LEFT, jobStr);
			resetColor();

			std::string statVal = std::to_string(dwarfStats.at(dwarfSelected).at(stat));

			// Print the stats value
			terminal_print_ext(statValueIndent, tileIndent, panelWidth, panelHeight, TK_ALIGN_LEFT, statVal.c_str());

			tileIndent += 1;
			++statCounter;
		}

		// Need for loop printing Combat
		// stats after labor stats here!

		terminal_refresh();
		resetColor();

		statSelected = dwarfStatOptionsInput(dwarfSelected, statsSelected, statSelected, statCounter);

		//if (statSelected == EXIT_CODE)
			//return;	
	} while (statsSelected);
}

int MainMenu::dwarfStatOptionsInput(int dwarfSelected, bool &statsSelected, int statSelected, int maxStats)
{
	int key;

	if (terminal_peek())
		key = terminal_read();
	else
		return statSelected;

	upOrDownInput(key, statSelected);

	switch (key)
	{
	case TK_ESCAPE:
		statsSelected = false;
		return EXIT_CODE;

	case TK_EQUALS:
		// Fall Through
	case TK_KP_PLUS:
		++dwarfStats.at(dwarfSelected).at(statSelected + 1);
		break;

	case TK_MINUS:
		--dwarfStats.at(dwarfSelected).at(statSelected + 1);
		break;

	case TK_LEFT:
		statsSelected = false;
		return EXIT_CODE;

	}

	// Handle wrapping
	if (statSelected > maxStats)
		statSelected = 0;
	else if (statSelected < 0)
		statSelected = maxStats - 1;

	return statSelected;
}
