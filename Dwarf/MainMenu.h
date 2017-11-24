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
		START_GAME_O,
		CREATE_WORLD_O,
		SETTINGS_O,
		QUIT_O,

		// Codes for different 
		// actions on MainMenu::render() return
		EXIT_CODE = 10001,
		START_CODE
	};

	// Default map width's and heights
	int mapWidth = 80;
	int mapHeight = 80;

private:

	int panelWidth;
	int panelHeight;

	void setColor();

	// Main Menu functions
	void resetColor();
	void determineHighlight(int h, int num);
	int mainMenuInput(int selected);
	void upOrDownInput(int key, int & selected);

	int startGame();

	
	bool pickDwarves();
	int pickDwarvesInput(int selected, int maxNumber);

	// Vector of desired stats for dwarves
	std::vector<std::vector<int>> dwarfStats;
	void printDwafStatOptions(int dwarfSelected);
	int dwarfStatOptionsInput(int dwarfSelected, int selected, int maxStats);

	//void settings();
};

