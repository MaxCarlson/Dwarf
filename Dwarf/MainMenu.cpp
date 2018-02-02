#include "MainMenu.h"
#include "BearLibTerminal.h"
#include "Engine.h"
#include "EntityFactory.h"
#include "ECS\Components\JobComponent.h"
#include "Drawing\draw.h"
#include "Map\building\PlanetBuilding.h"
#include "WorldGeneration.h"

#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::experimental::filesystem;
using namespace draw;

int MainMenu::render()
{
	panelWidth = terminal_state(TK_WIDTH);
	panelHeight = terminal_state(TK_HEIGHT);

	// Resize each dwarves stat vector
	// THIS SHOULD BE MOVED ELSEWHERE!!
	dwarfStats.resize(NumberOfDwaves);
	static const std::size_t totalStatNumber = listOfAllJobsByIndex.size(); // + listOfAllCombatStats.Size()
	{
		int statCounterDwarf = 0;
		for (auto& dwarVec : dwarfStats)
			dwarfStats[statCounterDwarf++].resize(totalStatNumber);
	}

	int selected = 0;

	while (true)
	{
		std::vector<std::string> menuOptions = { "Load Game", "Create World", "Settings", "Quit" };

		int code = listHandler<std::string, false>(menuOptions, selected, TK_ALIGN_CENTER, 0, panelHeight / 2 - 15, 5, true);	

		if (code == IN_ENTER)
		{
			switch (selected)
			{
			case 0:
				code = loadWorld();
				break;

			case 1:
				//code = pickDwarves();
				code = mainBuildPlanet();
				break;

			case 2:
				//settings();
				break;

			case 3:
				return EXIT_CODE;

			case 4:
				break;
			}
		}

		if (code == IN_EXIT)
		{
			return EXIT_CODE;
		}
		else if (code == START_CODE)
			return START_CODE;

		else if (code == LOAD_WORLD)
			return LOAD_WORLD;
	}
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

int MainMenu::pickDwarves()
{
	int selected = 0;

	// Do we want to be scrolling through
	// stats or dwaves?
	bool statsSelected = false;

	while (true)
	{
		terminal_clear();

		terminal_print_ext(0, 0, panelWidth, panelHeight, TK_ALIGN_LEFT, "Dwaves");

		std::vector<std::string> dwarfNames;
		for (int d = 0; d < NumberOfDwaves; ++d)
		{
			// Rudimentry naming
			std::string ds = "Dwarf ";
			ds += d + 1 + '0';
			dwarfNames.push_back(ds);
		}
		dwarfNames.push_back("Start Now!");

		int code = listHandler<std::string, false>(dwarfNames, selected, TK_ALIGN_LEFT, 0, 2, 1, false, true);

		if(selected < NumberOfDwaves)
			printDwafStatOptions(selected, statsSelected);


		// Local exit of this menu to main menu
		if (code == IN_EXIT)
			return IN_NOTHING;

		else if (code == IN_ENTER && selected == NumberOfDwaves)
		{
			finalizeDwarfPicks();
			return START_CODE;
		}
		else if (code == IN_ENTER)
			statsSelected = true;
	}
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

		/*
		std::vector<std::string> jobsVec;
		for (int stat : listOfAllJobsByIndex)
		{
			if (stat == 0) continue;
			jobsVec.push_back(listOfAllJobsByString[stat]);
		}
		int statCounter = jobsVec.size() - 1;

		int code = listHandler<std::string, true>(jobsVec, statSelected, TK_ALIGN_LEFT, statIndent, 2, 1, false, true, true);

		if (code == IN_EXIT)
		{
			statsSelected = false;
		}
		else if (code == IN_ENTER)
			statsSelected = true;
		*/
		///*
		// Labor Stats
		int tileIndent = 2;
		int statCounter = -1; // -1 due to first job being Job::NONE
		for (int stat : listOfAllJobsByIndex)
		{
			if (stat == int(Jobs::NONE))
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
		//*/
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

		createDwarf(dwarf);

		++dwarfCounter;
	}
}

// Loading already created region

int MainMenu::loadWorld()
{
	int selected = 0;

	std::string dirpath = "Saves";
	std::vector<std::string> paths;
	for (auto & p : fs::directory_iterator(dirpath))
	{
		paths.push_back(p.path().string());
		//terminal_print_ext(10, 10, panelWidth, panelHeight, TK_ALIGN_LEFT, p.path().c_str());
	}

	if (!paths.size())
		paths.push_back("No save games on File! Press Esc");

	while (true)
	{
		int code = listHandler<std::string, true>(paths, selected, TK_ALIGN_LEFT, 7, 7, 2, true);

		if (code == IN_EXIT)
			return IN_NOTHING;

		else if (code == IN_ENTER)
		{
			// Set the path so engine
			// can load the world when it's 
			// initialized
			mapPath = paths[selected];
			return LOAD_WORLD;
		}
	}


	
	return IN_NOTHING; 
}

int MainMenu::mainBuildPlanet()
{
	static const std::vector<std::string> yesno = { "Yes", "No", "Exit" };
	static const std::vector<std::string> nplanetSize = { "Small", "Medium", "Large" };
	static const std::vector<std::string> nregionSize = { "Small", "Medium", "Large" };
	static const std::vector<std::string> nwaterAmount = { "None", "A little", "Lots" };

	std::vector<int> planetSizes = { 64, 128, 256 };

	while (true)
	{
		generateWorld("seed", planetSizes[0], planetSizes[0], { 80, 80, 128 }, 3, 3, 7);

		terminal_print_ext(1, 5, 0, 0, TK_ALIGN_LEFT, "Is World Acceptable?");

		int code = 0, selected = 0;

		while (code == IN_NOTHING)
		{
			code = listHandler<std::string, true>(yesno, selected, TK_ALIGN_LEFT, 1, 7, 1, false);
		}

		if (code == IN_ENTER && selected == 0)
		{
			bool accepted = chooseEmbark(px, py);

			if (accepted)
			{
				finalizeDwarfPicks();
				return START_CODE;
			}			
		}
		else if (code == IN_ENTER && selected == 2)
			return IN_NOTHING;
	}


	return IN_NOTHING;
}


