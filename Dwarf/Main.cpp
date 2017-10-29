#include "include/libtcod.hpp"

#include "Actor.h"
#include "Map.h"
#include "Engine.h"

Engine engine(80, 50);

int main()
{
	int playerX = 40, playerY = 25;
	

	while (!TCODConsole::isWindowClosed())
	{
		engine.update();
		engine.render();

		TCODConsole::flush();
	}


	return 0;
}