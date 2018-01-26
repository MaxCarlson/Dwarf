#pragma once
#include <vector>


struct GameInit
{
	//std::vec
};

// Handles all input and rendering
// until the local map is generated
class MainMenu
{
public:

	int render();

	enum MenuOptions
	{
		// Visual / UI
		// Option Enums
		START_GAME_M,
		CREATE_WORLD_M,
		SETTINGS_M,
		QUIT_M,

		LOAD_WORLD,
		BUILD_PLANET,

		// Codes for different 
		// actions on MainMenu::render() return
		EXIT_CODE = 10001,
		START_CODE
	};

	// Default map width's and heights
	int mapWidth = 80;
	int mapHeight = 80;

	int px, py;

	std::string mapPath;

private:

	int panelWidth;
	int panelHeight;

	// Main Menu functions
	void upOrDownInput(int key, int & selected);

	int loadWorld();

	int mainBuildPlanet();

	// Dwarf picking functions
	// UI and gui Below
	const int NumberOfDwaves = 7;
	// Vector of desired stats for dwarves
	std::vector<std::vector<int>> dwarfStats;

	int pickDwarves();

	int availablePoints = 30;
	void printDwafStatOptions(int dwarfSelected, bool &statsSelected);
	int dwarfStatOptionsInput(int dwarfSelected, bool &statsSelected, int selected, int maxStats);
	void finalizeDwarfPicks();

	//void settings();
};


