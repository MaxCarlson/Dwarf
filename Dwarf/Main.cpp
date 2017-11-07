#include "include/libtcod.hpp"


#include "Map.h"
#include "Engine.h"

// Numbers a screen width and height, make more 
// dynamic eventually
Engine engine(129, 129);

int main()
{

	while (!TCODConsole::isWindowClosed())
	{
		engine.update();
		engine.render();

		TCODConsole::flush();
	}


	return 0;
}