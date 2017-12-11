#include "Input.h"

#include "Engine.h"
#include "Map/Map.h"
#include "Map/MapRender.h"

#include "BearLibTerminal.h"

// For testing mining
#include "Designations.h"
#include "ECS\Messages\recalculate_mining_message.h"

// For testing items
#include "Raws\Defs\ItemDefs.h"
#include "ECS\Components\Item.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\Components\PositionComponent.h"
#include "ECS\Messages\pick_map_changed_message.h"

using namespace region;

/// ######################## MAKE This into a System!!!

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

	if (engine->current_game_state == Engine::PLAY
		|| engine->current_game_state == Engine::PAUSED)
	{
		switch (keyPress)
		{
			// Camera
			// Move camera up or down z Levels
		case TK_COMMA:
			engine->map->mapRenderer->incrementZLevel(-1);
			break;

		case TK_PERIOD:
			engine->map->mapRenderer->incrementZLevel(1);
			break;

			// Directional movement for camera
		case TK_UP:
			engine->map->mapRenderer->moveCamera(MapRender::NORTH);
			break;

		case TK_DOWN:
			engine->map->mapRenderer->moveCamera(MapRender::SOUTH);
			break;

		case TK_RIGHT:
			engine->map->mapRenderer->moveCamera(MapRender::EAST);
			break;

		case TK_LEFT:
			engine->map->mapRenderer->moveCamera(MapRender::WEST);
			break;


			//default: break;
		}

		int xx = terminal_state(TK_MOUSE_X);
		int yy = terminal_state(TK_MOUSE_Y);

		if (keyPress == TK_MOUSE_LEFT)
		{
			Coordinates co1 = { xx, yy, engine->map->mapRenderer->currentZLevel };

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

			engine->world.emit(recalculate_mining_message{});

		}
		else if (keyPress == TK_MOUSE_RIGHT)
		{
			Entity e = engine->world.createEntity();
			e.addComponent<Item>();
			e.addComponent<RenderComponent>();
			e.addComponent<PositionComponent>(Coordinates{ xx, yy, engine->map->mapRenderer->currentZLevel });

			auto& it = e.getComponent<Item>();
			it.catagory.set(TOOL_DIGGING);

			auto& rend = e.getComponent<RenderComponent>();

			rend.ch = 55;
			rend.colorStr = "blue";
			rend.terminalCode = 0xE300;

			e.activate();

			engine->world.emit(pick_map_changed_message{});
		}
		else if (keyPress == TK_ESCAPE)
		{
			if (engine->current_game_state == Engine::ESC_MENU)
			{

			}
			pauseGame();
			engine->current_game_state = Engine::TO_MAIN_MENU;
		}
		else if (keyPress == TK_SPACE)
		{
			pauseGame();
		}
	}

}

void Input::pauseGame()
{
	if (engine->current_game_state != Engine::PLAY)
		engine->current_game_state = Engine::PLAY;

	else
		engine->current_game_state = Engine::PAUSED;
}
