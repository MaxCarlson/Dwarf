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
	terminal_set("window.title='Dwarf'; window.size=155x65; window.cellsize=auto");
	terminal_set("U+E100: ../Obsidian_16x16.png, size=16x16");
	terminal_set("U+E200: ../Spacefox_16x16Dibujor01.png, size=16x16");
	terminal_composition(true);

	while (!TCODConsole::isWindowClosed())
	{
		terminal_clear();
		engine.update();
		engine.render();

		TCODConsole::flush();

		terminal_refresh();
		
	}


	return 0;
}