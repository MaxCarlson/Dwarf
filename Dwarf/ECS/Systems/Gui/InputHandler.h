#pragma once
#include "../../Systems.h"

class InputHandler : public System<Requires<>>
{
public:
	void update();

private:
	int getMouseIdx();

	void designate(const int key);
	void buildMenu(const int key);
	void createItem(const int key);
	void reactions(const int key);
	void stockpiles(const int key);

	int mouseX;
	int mouseY;
};

