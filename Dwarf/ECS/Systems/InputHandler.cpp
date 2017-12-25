#include "InputHandler.h"
#include "../BearLibTerminal.h"
#include "../Engine.h"
#include "../Gui.h"
#include "../Messages/designation_message.h"
#include "../Map/Tile.h"

void InputHandler::update()
{
	int key;

	mouseX = terminal_state(TK_MOUSE_X);
	mouseY = terminal_state(TK_MOUSE_Y);

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

	if (engine->gui.state == Gui::MAIN)
		switch (key)
		{
		case TK_ESCAPE:
			engine->gui.state = Gui::ESC_MENU;
			break;

		case TK_B:
			engine->gui.state = Gui::BUILD;
			break;

		case TK_D:
			engine->gui.state = Gui::DESIGNATE;
			break;

		case TK_O:
			engine->gui.state = Gui::ORDERS;
			break;
		}

	else if (engine->gui.state == Gui::BUILD)
	{
		if (key == TK_ESCAPE)
			engine->gui.state = Gui::MAIN;
		else
			buildMenu(key);
	}

	else if (engine->gui.state == Gui::DESIGNATE)
	{
		designate_state = designate_states::MINE_D;
		if (key == TK_ESCAPE)
		{
			engine->gui.state = Gui::MAIN;
			designate_state = designate_states::NONE_D;
		}

		else if (key == TK_H)
			designate_state = designate_states::CHANNEL_D;		
		
		designate(key);
	}

	else if (engine->gui.state == Gui::ORDERS)
	{
		if (key == TK_ESCAPE)
			engine->gui.state = Gui::MAIN;
	}

	else if (engine->gui.state == Gui::ESC_MENU)
	{
		if (key == TK_ESCAPE)
			engine->gui.state = Gui::MAIN;
	}
	
}

void InputHandler::designate(const int key)
{
	if (key == TK_MOUSE_LEFT)
	{
		int clickIdx = getIdx({ mouseX, mouseY, engine->mapRenderer->currentZLevel });
		if (desig_idx == 0)
		{
			desig_idx = clickIdx;
		}
		else
		{
			emit(designation_message{ std::make_pair(desig_idx, clickIdx), designate_state });
			desig_idx = 0;
		}
	}
}

void InputHandler::buildMenu(const int key)
{

}
