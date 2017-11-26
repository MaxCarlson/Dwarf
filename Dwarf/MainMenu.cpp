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

		determineHighlight(selected, START_GAME_M);
		terminal_print_ext(0, panelHeight / 2 - 15, panelWidth, panelHeight, TK_ALIGN_CENTER, "Start Game");

		determineHighlight(selected, CREATE_WORLD_M);
		terminal_print_ext(0, panelHeight / 2 - 10, panelWidth, panelHeight, TK_ALIGN_CENTER, "Create World");

		determineHighlight(selected, SETTINGS_M);
		terminal_print_ext(0, panelHeight / 2 - 5, panelWidth, panelHeight, TK_ALIGN_CENTER, "Settings");

		determineHighlight(selected, QUIT_M);
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
		case START_GAME_M:
			return startGame();
			break;

		case CREATE_WORLD_M:
			break;

		case SETTINGS_M:
			//settings();
			break;

		case QUIT_M:
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

	// Resize each dwarves stat vector
	// THIS SHOULD BE MOVED ELSEWHERE!!
	dwarfStats.resize(NumberOfDwaves);
	static const int totalStatNumber = listOfAllJobsByIndex.size(); // + listOfAllCombatStats.Size()
	{
		int statCounterDwarf = 0;
		for (auto& dwarVec : dwarfStats)
			dwarfStats[statCounterDwarf++].resize(totalStatNumber);
	}

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
		for (int d = 0; d < NumberOfDwaves + 1; ++d)
		{
			determineHighlight(selected, d);

			// Rudimentry naming
			std::string ds = "Dwarf ";
			ds += d + 1 + '0';

			if(d < NumberOfDwaves)
				terminal_print_ext(0, tileIndent, panelWidth, panelHeight, TK_ALIGN_LEFT, ds.c_str());

			else
				terminal_print_ext(0, tileIndent + 1, panelWidth, panelHeight, TK_ALIGN_LEFT, "Start Now!");
			
			tileIndent += 1;
		}

		if(selected < NumberOfDwaves)
			printDwafStatOptions(selected, statsSelected);

		terminal_refresh();
		resetColor();

		selected = pickDwarvesInput(selected, statsSelected, NumberOfDwaves + 1);

		// Local exit of this menu to main menu
		if (selected == EXIT_CODE)
			return false;
		else if (selected == START_CODE)
			return true;
	}
}

int MainMenu::pickDwarvesInput(int dwarfSelected, bool &statsSelected, int maxNumber)
{
	const int key = terminal_read();

	upOrDownInput(key, dwarfSelected);

	switch (key)
	{
	case TK_ESCAPE:
		return EXIT_CODE;

	case TK_ENTER: 
		if (dwarfSelected == maxNumber - 1) 
		{
			finalizeDwarfPicks();
			return START_CODE;
		}
		break;


	case TK_RIGHT:
		statsSelected = true;
		break;
	}

	// Wrap scrolling highlighting
	if (dwarfSelected >= maxNumber)
		dwarfSelected = 0;
	else if (dwarfSelected < 0)
		dwarfSelected = maxNumber - 1;

	return dwarfSelected;
}

void MainMenu::printDwafStatOptions(int dwarfSelected, bool &statsSelected)
{
	int statSelected = 0;
	static const int statIndent = 15;
	static const int statValueIndent = 25;
		
	do
	{
	// Get rid of this goto!!
	CLEAR_HIGHLIGHT_BEFORE_EXIT:

		terminal_clear_area(12, 0, panelWidth, panelHeight);

		// Availbile points to spend 
		std::string points = "Availbile Points ";
		points += std::to_string(availablePoints);
		terminal_print_ext(15, 0, panelWidth, panelHeight, TK_ALIGN_LEFT, points.c_str());

		// Labor Stats
		int tileIndent = 2;
		int statCounter = -1; // -1 due to first job being Job::NONE
		for (int stat : listOfAllJobsByIndex)
		{
			if (stat == Job::Jobs::NONE)
				continue;

			// If the stats are selected instead of dwaves
			// figure out which stat to highlight
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

		// If we're exiting statSelection
		// Run through the loop one more time
		// to clear the highlight off the Stat
		if (statSelected == EXIT_CODE)
		{
			statSelected = -1;
			goto CLEAR_HIGHLIGHT_BEFORE_EXIT;
		}

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
	// Break back to dwarf selection
	case TK_ESCAPE: case TK_LEFT:
		statsSelected = false;
		return EXIT_CODE;

	// Increment Dwarf Starting Stat
	case TK_KP_PLUS: case TK_EQUALS:
		if (availablePoints
			&& dwarfStats.at(dwarfSelected).at(statSelected + 1) < 20)
		{
			++dwarfStats.at(dwarfSelected).at(statSelected + 1);
			--availablePoints;
		}		
		break;

	// Decrement Dwarf Starting Stat
	case TK_MINUS: case TK_KP_MINUS:
		if (dwarfStats.at(dwarfSelected).at(statSelected + 1))
		{
			--dwarfStats.at(dwarfSelected).at(statSelected + 1);
			++availablePoints;
		}		
		break;
	}

	// Handle wrapping
	if (statSelected > maxStats)
		statSelected = 0;
	else if (statSelected < 0)
		statSelected = maxStats - 1;

	return statSelected;
}

void MainMenu::finalizeDwarfPicks()
{
	std::vector<DwarfCreationObj> dwarvesInit(NumberOfDwaves);

	// Loop through potential dwarves 
	// add stats to dwarCreationObj's
	// and create them
	int dwarfCounter = 0;
	for (auto& dwarf : dwarvesInit)
	{		
		dwarf.skillPoints.resize(dwarfStats[0].size());
		// To dwarfCreationObj add 35 skill points per
		// point invested in Should this be in EntityFactory??
		int statCounter = 0;
		for (auto& dwarfSkillPoints : dwarf.skillPoints)
		{
			dwarfSkillPoints = dwarfStats[dwarfCounter][statCounter] * 35;
			++statCounter;
		}

		// Need to name dwarves here

		engine.factory.createDwarf(dwarf);

		++dwarfCounter;
	}
}
