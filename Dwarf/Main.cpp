#include "include/libtcod.hpp"
#include "BearLibTerminal.h"

#include "Map.h"
#include "Engine.h"
#include "MainMenu.h"


// Numbers a screen width and height, make more 
// dynamic eventually
Engine engine;

// temp variable for map size
static const int mapSize = 80;

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

	while (true)
	{
		MainMenu menu;
		menu.render();

		engine.init(mapSize, mapSize);

		// Start the game loop.
		engine.run();
	}

	return 0;
}