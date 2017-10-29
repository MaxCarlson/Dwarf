#pragma once
#include "include\libtcod.hpp"


class Gui
{
public:
	Gui();
	~Gui();
	void render();
	void message(const TCODColor & col, const char * text, ...);

protected:
	TCODConsole *con;

	struct Message {
		char * text;
		TCODColor col;
		Message(const char * text, const TCODColor & col);
		~Message();
	};
	TCODList<Message *> log;

	void renderBar(int x, int y, int width, const char * name, int val, int maxVal, const TCODColor & barColor, const TCODColor &backColor);
};

