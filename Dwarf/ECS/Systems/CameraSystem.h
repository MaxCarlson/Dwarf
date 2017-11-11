#pragma once

#include "../Systems.h"
#include "../Components/KeyBoardComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/CameraComponent.h"
#include "../../Engine.h"
#include "../../Map.h"
#include "../BearLibTerminal.h"
#include <string>

// Currently provides just the vertical movement 
// for camera
class CameraSystem : public System<Requires<KeyboardComponent, PositionComponent, CameraComponent>> // Possibly require different components in future? // Must add flag for in menu option once there are menu's
{
public:

	void update()
	{
		auto entities = getEntities();

		for (auto e : entities)
		{
			//const auto& key = e.getComponent<KeyboardComponent>();
			auto& co = e.getComponent<PositionComponent>().co;

			int keyPress = terminal_read();

			if (keyPress == TK_MOUSE_MOVE)
				break;
			
			switch (keyPress)
			{

			// Move camera up or down z Levels
			case TK_COMMA:
				if (engine.map->mapRenderer->incrementZLevel(-1))
					co.z -= 1;
				break;

			case TK_PERIOD:
				if (engine.map->mapRenderer->incrementZLevel(1))
					co.z += 1;
				break;

			// Directional movement for camera
			case TK_UP:
				engine.map->mapRenderer->offsetY -= 1;
				break;

			case TK_DOWN:
				engine.map->mapRenderer->offsetY += 1;
				break;

			case TK_RIGHT:
				engine.map->mapRenderer->offsetX += 1;
				break;


			case TK_LEFT:
				engine.map->mapRenderer->offsetX -= 1;
				break;
				

			case TK_MOUSE_SCROLL:
				handleMouseWheel(keyPress, e);
				break;

			default: break;
			}
			
		}
	}

private:

	void handleMouseWheel(int key, Entity & entity) // Not working!!
	{
		// Amount of steps mouse wheel has scrolled
		int amount = terminal_state(TK_MOUSE_WHEEL);

		auto& cam = entity.getComponent<CameraComponent>();

		int newCellSize = cam.currentCellSize += amount;

		std::string cellStr = "window.cellsize=";

		cellStr += std::to_string(newCellSize);
		cellStr += 'x';
		cellStr += std::to_string(newCellSize);

		const char * cstr = cellStr.c_str();

		terminal_set(cstr);
	}
};

/*
// Libtcod code

int dx = 0, dy = 0;
switch (key.lastKeyPressed->vk)
{
// Change z level of camera
case TCODK_TEXT:
switch (key.lastKeyPressed->text[0]) { // For single entry non coded keys
case '<':
if (engine.map->incrementZLevel(-1))
co.z -= 1;

break;

case '>':
if(engine.map->incrementZLevel(1))
co.z += 1;

break;
}
break;

default: break;
}
*/
//