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
	int keyPress;

	// Don't block engine waiting for input
	if (terminal_peek())
		keyPress = terminal_read();
	else
		return;

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
	case TK_SPACE:
		while (terminal_read() != TK_SPACE) {}
	

// Mouse scrolling
	case TK_MOUSE_SCROLL:
		//handleMouseWheel(keyPress, e);
		break;

	default: break;
	}
}
