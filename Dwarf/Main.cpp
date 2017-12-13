#include "include/libtcod.hpp"
#include "BearLibTerminal.h"

#include "Map/Map.h"
#include "Engine.h"
#include "MainMenu.h"


// Numbers a screen width and height, make more 
// dynamic eventually
std::unique_ptr<Engine> engine;

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
		engine = std::make_unique<Engine>();

		MainMenu menu;
		int code = menu.render();

		if (code == MainMenu::EXIT_CODE)
			break;

		else if (code == MainMenu::START_CODE)
			engine->newGame(menu.mapWidth, menu.mapHeight);
			

		else if (code == MainMenu::LOAD_WORLD)
			engine->loadGame(menu.mapPath);
			
		

		

		// Start the game loop.
		engine->run();
	}

	return 0;
}