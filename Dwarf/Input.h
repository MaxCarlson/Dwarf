#pragma once
class Input
{
public:
	Input();
	~Input();

	void read();

	void pauseGame();

	int drawEscMenu(int& selected);
	void drawSaveMenu();
};

