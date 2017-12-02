#include "Input.h"

#include "Engine.h"
#include "Map/Map.h"
#include "Map/MapRender.h"

#include "BearLibTerminal.h"
#include "Designations.h"
#include "ECS\Messages\recalculate_mining_message.h"

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


// Test
	case TK_MOUSE_LEFT:
		int xx = terminal_state(TK_MOUSE_X);
		int yy = terminal_state(TK_MOUSE_Y);

		Coordinates co1 = { xx, yy, engine.map->mapRenderer->currentZLevel };

		for (int i = 0; i < 10; ++i)
		{
			Coordinates co = co1;
			co.x = co1.x + i;
			for (int j = 0; j < 10; ++j)
			{
				co.y = co1.y + j;
				designations->mining.emplace(getIdx(co), 1);
			}
		}		

		engine.world.emit(recalculate_mining_message{});

		break;
	

	//default: break;
	}
}
