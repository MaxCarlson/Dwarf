#include "include/libtcod.hpp"
#include "BearLibTerminal.h"

//#define  _CRT_SECURE_NO_WARNINGS 
//#define _CRT_SECURE_NO_WARNINGS_GLOBALS
//#ifdef _MSC_VER
//#define _CRT_SECURE_NO_WARNINGS
//#endif

#include "Map.h"
#include "Engine.h"

// Numbers a screen width and height, make more 
// dynamic eventually
Engine engine(129, 129);

int main()
{
	terminal_open();

	while (!TCODConsole::isWindowClosed())
	{
		terminal_refresh();
		engine.update();
		engine.render();

		TCODConsole::flush();
	}


	return 0;
}