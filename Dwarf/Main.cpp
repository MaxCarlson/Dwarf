#include "include/libtcod.hpp"
#include "BearLibTerminal.h"
#include "Map/Map.h"
#include "Engine.h"
#include "MainMenu.h"
#include "Raws\raws.h"
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <DwarfRender.h>

// Numbers a screen width and height, make more 
// dynamic eventually
std::unique_ptr<Engine> engine;

int main()
{
	// BearslibTerminal Init stuff
	terminal_open();
	terminal_set("window.title='Dwarf'; window.size=80x80; window.cellsize=16x16"); // ~~ Make lua config file

	// Move these into lua config files that don't require compiling
	// so outside users can modify tilesets/etc
	terminal_set("U+E100: ../Spacefox_16x16Dibujor01.png, size=16x16");
	terminal_set("U+E200: ../tools+weapons.png, size=16x16");
	terminal_set("U+E300: ../dorenitems16x16.png, size=16x16");
	terminal_set("U+E400: ../dwarves.png, size=16x16");
	terminal_set("U+E900: ../shadows.png, size=16x16");

	terminal_set("input.filter = [keyboard, mouse+]");
	terminal_composition(true);

	dfr::config_r cf("SFML WINDOW", "DwarfTex.png", 40, 40, "font");
	dfr::registerFont("font", "DwarfTex.png", 16, 16);
	dfr::initInputs(16, 16);

	dfr::init(cf);

	ImGui::SFML::Init(*dfr::getWindow());
	//getWindow()->resetGLStates();

	dfr::terminal->setChar(0, 0, { 'b',{ 55, 55, 0 },{ 0, 0, 233 } });
	dfr::terminal->setChar(2, 0, { 196,{ 55, 55, 0 },{ 0, 0, 233 } });
	dfr::terminal->setChar(3, 0, { 205,{ 55, 55, 0 },{ 0, 0, 233 } });
	dfr::terminal->setChar(25, 0, { 570,{ 255, 55, 0 },{ 233, 55, 233 } });


	dfr::run([](double d) {});

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
			engine->newGame(menu.px, menu.py, menu.mapWidth, menu.mapHeight);		

		else if (code == MainMenu::LOAD_WORLD)
			engine->loadGame(menu.mapPath);
			
		

		

		// Start the game loop.
		engine->run();
	}

	return 0;
}