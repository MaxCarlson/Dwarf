#pragma once

// Handles all input and rendering
// until the local map is generated
class MainMenu
{
public:

	int render();

	enum MenuOptions
	{
		START_GAME,
		CREATE_WORLD,
		SETTINGS,
		QUIT,
		EXIT_CODE
	};

private:

	void setColor();
	void resetColor();
	void determineHighlight(int h, int num);

	int mainMenuInput(int selected);
};

