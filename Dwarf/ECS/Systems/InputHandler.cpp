#include "InputHandler.h"
#include "../BearLibTerminal.h"
#include "../Engine.h"
#include "../Gui.h"
#include "../Messages/designation_message.h"
#include "../Map/Tile.h"
#include "../Raws/ItemRead.h"
#include "../Raws/raws.h"

// Desig statics
static uint8_t designateState = designation_message::MINING;
static int designateIdx = 0;

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

		case TK_I:
			engine->gui.state = Gui::CREATE_ITEM;
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
		if (key == TK_ESCAPE)
		{
			engine->gui.state = Gui::MAIN;
			designateState = designation_message::MINING;
			designateIdx = 0;
		}

		else if (key == TK_H)
			designateState = designation_message::CHANNELING;
		
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
	
	else if (engine->gui.state == Gui::CREATE_ITEM)
	{
		if (key == TK_ESCAPE)
			engine->gui.state = Gui::MAIN;

		createItem(key);
	}
}

void InputHandler::designate(const int key)
{
	if (key == TK_MOUSE_LEFT)
	{
		int clickIdx = getIdx({ mouseX, mouseY, engine->mapRenderer->currentZLevel });
		if (!designateIdx)
		{
			designateIdx = clickIdx;
		}
		else
		{
			emit(designation_message{ std::make_pair(designateIdx, clickIdx), designateState });
			designateIdx = 0;
		}
	}
}

void InputHandler::buildMenu(const int key)
{

}

void InputHandler::createItem(const int key)
{
	static auto itemTags = get_all_item_tags();

	if (key == TK_PAGEDOWN)
		++engine->gui.itemSelected;

	else if (key == TK_PAGEUP)
		--engine->gui.itemSelected;

	else if (key == TK_ENTER || key == TK_MOUSE_LEFT)
	{
		const auto tag = itemTags[engine->gui.itemSelected];
		const Coordinates co = { mouseX, mouseY, engine->mapRenderer->currentZLevel }; // Add material choices once needed

		spawnItemOnGround(tag, 1, co);
	}
}
