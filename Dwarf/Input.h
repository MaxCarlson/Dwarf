#pragma once
class Input
{
public:
	Input();
	~Input();

	void read();

	void pauseGame();

	void normal(const int key);
	void build(const int key);
	void orders(const int key);
	void esc(const int key);

	int drawEscMenu(int& selected);
	void drawSaveMenu();
};

