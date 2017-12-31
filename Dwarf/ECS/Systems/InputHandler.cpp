#include "InputHandler.h"
#include "../BearLibTerminal.h"
#include "../Engine.h"
#include "../Gui.h"
#include "../Map/Tile.h"
#include "../Raws/ItemRead.h"
#include "../Raws/raws.h"
#include "../Raws/Buildings.h"
#include "../Messages/designation_message.h"
#include "../Messages/designate_building_message.h"
#include "../ECS/Messages/request_new_stockpile_message.h"
#include "../Designations.h"
#include "../Raws/DefInfo.h"
#include "../Raws/Defs/ItemDefs.h"

// temp till things are placed elsewhere
#include "../ECS/Messages/pick_map_changed_message.h"


// Desig statics
static uint8_t designateState = designation_message::MINING;
static int designateIdx = 0;


void testEsc(const std::function<void()>& func)
{

}

void handleScroll(const int key, int down = TK_PAGEDOWN, int up = TK_PAGEUP)
{
	if (key == down)
		++engine->gui.itemSelected;

	else if (key == up)
		--engine->gui.itemSelected;
}

int InputHandler::getMouseIdx()
{
	return getIdx({ mouseX, mouseY, engine->mapRenderer->currentZLevel });
}

void InputHandler::update()
{
	int key;

	mouseX = terminal_state(TK_MOUSE_X);
	mouseY = terminal_state(TK_MOUSE_Y);

	// Don't block engine waiting for input
	while (terminal_peek())
	{
		key = terminal_read();

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
		{
			designateIdx = 0;

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

			case TK_M:
				engine->gui.state = Gui::REACTIONS;
				break;

			case TK_P:
				engine->gui.state = Gui::STOCKPILES;
				break;
			}
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
			else
				createItem(key);
		}

		else if (engine->gui.state == Gui::REACTIONS)
		{
			if (key == TK_ESCAPE)
				engine->gui.state = Gui::MAIN;

			else
				reactions(key);
		}

		else if (engine->gui.state == Gui::STOCKPILES)
		{
			if (key == TK_ESCAPE)
				engine->gui.state = Gui::MAIN;
			else
				stockpiles(key);
		}
	}
}

void InputHandler::designate(const int key)
{
	handleScroll(key);

	if (key == TK_ENTER || key == TK_MOUSE_LEFT)
	{
		int clickIdx = getMouseIdx();
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
	handleScroll(key);

	if (key == TK_ENTER || key == TK_MOUSE_LEFT)
	{
		const auto tag = defInfo->buildingTags[engine->gui.itemSelected];
		int clickIdx = getIdx({ mouseX, mouseY, engine->mapRenderer->currentZLevel });

		emit(designate_building_message{ *getBuilding(tag), clickIdx });
	}
}

void InputHandler::createItem(const int key)
{
	handleScroll(key);

	if (key == TK_ENTER || key == TK_MOUSE_LEFT)
	{
		const auto tag = defInfo->itemTags[engine->gui.itemSelected];
		const Coordinates co = { mouseX, mouseY, engine->mapRenderer->currentZLevel }; // Add material choices once needed

		spawnItemOnGround(tag, 1, co);

		// For the moment
		if(tag == "pickaxe")
			emit(pick_map_changed_message{});
	}
}

void InputHandler::reactions(const int key)
{
	handleScroll(key);

	if (key == TK_ENTER)
	{
		const auto tag = defInfo->availibleReactions[engine->gui.itemSelected];

		designations->workOrders.push_back(std::make_pair(1, tag));
	}
}

void InputHandler::stockpiles(const int key)
{
	handleScroll(key);

	if (key == TK_ENTER || key == TK_MOUSE_LEFT)
	{
		// Nothing designated yet, set idx
		if (!designateIdx)
		{
			designateIdx = getMouseIdx();
		}
		else
		{
			const auto tag = defInfo->stockpileTags[engine->gui.itemSelected];

			auto cata = getStockpileDef(tag);

			std::bitset<64> catagories;

			catagories.set(cata->index);

			emit(request_new_stockpile_message{ std::make_pair(designateIdx, getMouseIdx()), catagories });
		}	
	}
}
