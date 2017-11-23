#pragma once
class MainMenu
{
public:
	MainMenu();
	~MainMenu();

	void render();

private:

	void setColor();
	void resetColor();
	void determineHighlight(int h, int num);

	int processInput(int selected);
};

