#include "Input.h"

#include "Engine.h"
#include "Map.h"
#include "MapRender.h"

#include "BearLibTerminal.h"


Input::Input()
{
}


Input::~Input()
{
}

void Input::read()
{
	int keyPress = terminal_read();

	switch (keyPress)
	{
// Camera
	// Move camera up or down z Levels
	case TK_COMMA:
		engine.map->mapRenderer->incrementZLevel(-1);
		break;

	case TK_PERIOD:
		engine.map->mapRenderer->incrementZLevel(1);
		break;

	// Directional movement for camera
	case TK_UP:
		engine.map->mapRenderer->moveCamera(MapRender::NORTH);
		break;

	case TK_DOWN:
		engine.map->mapRenderer->moveCamera(MapRender::SOUTH);
		break;

	case TK_RIGHT:
		engine.map->mapRenderer->moveCamera(MapRender::EAST);
		break;

	case TK_LEFT:
		engine.map->mapRenderer->moveCamera(MapRender::WEST);
		break;

// Gui
	case TK_TAB:
		// Increment gui size,
		// wrapping back to gui being off
		switch (engine.gui.guiState)
		{
		case Gui::OFF:
			engine.gui.guiState = Gui::SMALL;
			break;

		case Gui::SMALL:
			engine.gui.guiState = Gui::LARGE;
			break;

		case Gui::LARGE:
			engine.gui.guiState = Gui::OFF;

			// Make sure when resetting the Gui width to zero we don't
			// show anything off the edge of the map
			int panelWidth = terminal_state(TK_WIDTH);
			if(engine.map->mapRenderer->offsetX + engine.gui.horizontalOffset > panelWidth)
				engine.map->mapRenderer->offsetX -= engine.gui.horizontalOffset;

			break;
		}


// Mouse scrolling
	case TK_MOUSE_SCROLL:
		//handleMouseWheel(keyPress, e);
		break;

	default: break;
	}
}
