#include "include/libtcod.hpp"
#include "BearLibTerminal.h"
#include "Map/Map.h"
#include "Raws\raws.h"
#include "Globals\GlobalTerminals.h"
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <DwarfRender.h>

#include "Globals\MainFunction.h"
#include "MainLoops\MainMenuLoop.h"

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
	overlayTerm = dfr::term(1);

	// Run the main menu function
	// The function will change to another when the game is started
	dfr::run(MainFunction);
	


	return 0;
}