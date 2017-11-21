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
Engine engine(80, 80);

int main()
{
	// BearslibTerminal Init stuff
	terminal_open();
	terminal_set("window.title='Dwarf'; window.size=80x80; window.cellsize=16x16");
	terminal_set("U+E100: ../Obsidian_16x16.png, size=16x16");
	terminal_set("U+E200: ../Spacefox_16x16Dibujor01.png, size=16x16");
	terminal_set("U+E300: ../dwarves.png, size=16x16");
	terminal_set("input.filter = [keyboard, mouse+]");
	terminal_composition(true);

	// Start the game loop.
	// Need to add controls before game loop
	// for loading and start screens
	engine.run();


	return 0;
}