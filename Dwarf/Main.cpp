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

#include "Globals\MainFunction.h"
#include "MainLoops\MainMenuLoop.h"

// Numbers a screen width and height, make more 
// dynamic eventually
std::unique_ptr<Engine> engine;

void onResizeL1(dfr::Layer * l, int w, int h)
{
	l->w = w;
	l->h = h;
}

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

	loadRaws();
	
	///*
	dfr::config_r cf("SFML WINDOW", "../DwarfTex.png", 80, 80, "font");
	dfr::registerFont("font", "../DwarfTex.png", 16, 16);
	dfr::initInputs(16, 16);

	dfr::init(cf);

	ImGui::SFML::Init(*dfr::getWindow());
	//dfr::getWindow()->resetGLStates();

	MainFunction = MainMenuLoop::run;

	dfr::gui->addLayer(1, 0, 0, dfr::getWindow()->getSize().x, dfr::getWindow()->getSize().y, "font", onResizeL1, true); // Enum as layer terminal
	dfr::gui->getLayer(1)->terminal->setAlpha(215);

	while (true)
	{
		dfr::run(MainFunction);
	}


	//*/

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