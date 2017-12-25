#include "InputHandler.h"
#include "../BearLibTerminal.h"
#include "../Engine.h"

void InputHandler::update()
{
	int key;

	// Don't block engine waiting for input
	if (terminal_peek())
		key = terminal_read();
	else
		return;

	switch (key)
	{
	case TK_COMMA:
		engine->mapRenderer->incrementZLevel(-1); // Move this camera functionality into a system!! ~~ Along with map rendering ~ combine with render system
		break;

	case TK_PERIOD:
		engine->mapRenderer->incrementZLevel(1);
		break;

	case TK_UP:
		engine->mapRenderer->moveCamera(MapRender::NORTH);
		break;

	case TK_DOWN:
		engine->mapRenderer->moveCamera(MapRender::SOUTH);
		break;

	case TK_RIGHT:
		engine->mapRenderer->moveCamera(MapRender::EAST);
		break;

	case TK_LEFT:
		engine->mapRenderer->moveCamera(MapRender::WEST);
		break;
	}
}
