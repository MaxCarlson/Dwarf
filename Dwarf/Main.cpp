#include "include/libtcod.hpp"
#include "BearLibTerminal.h"
#include "Map/Map.h"
#include "Engine.h"
#include "MainMenu.h"
#include "Raws\raws.h"
#include "Drawing\imgui.h"

//#include <SFML\Graphics.hpp>

// Numbers a screen width and height, make more 
// dynamic eventually
std::unique_ptr<Engine> engine;

int main()
{
	//sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

	// BearslibTerminal Init stuff
	terminal_open();
	terminal_set("window.title='Dwarf'; window.size=80x80; window.cellsize=16x16"); // ~~ Make lua config file

	// Move these into lua config files that don't require compiling
	// so outside users can modify tilesets/etc
	terminal_set("U+E100: ../Obsidian_16x16.png, size=16x16");
	terminal_set("U+E200: ../Spacefox_16x16Dibujor01.png, size=16x16");
	terminal_set("U+E300: ../dwarves.png, size=16x16");
	terminal_set("U+E400: ../tools+weapons.png, size=16x16");

	terminal_set("input.filter = [keyboard, mouse+]");
	terminal_composition(true);

	loadRaws();

	while (true)
	{
		engine = std::make_unique<Engine>();
		engine->regComponents();

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