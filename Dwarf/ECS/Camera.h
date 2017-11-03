#pragma once
#include "EntityManager.h"
#include "../Tile.h"
#include "PositionComponent.h"


// This is just a placeholder until I add libtcod's camera utilitys
class CameraComponent : public Component
{
private:
	// What level is camera looking at, and by virtue of system
	// what level do we need to render?
	PositionComponent * position;
public:
	CameraComponent() = default;
	//CameraComponent() {};

	void init() override
	{
		position = &entity->getComponent<PositionComponent>();
	}

	void update() override
	{
		int dx = 0, dy = 0;
		switch (engine.lastKey.vk)
		{

		case TCODK_UP:    dy = -1; break;
		case TCODK_DOWN:  dy = 1; break;
		case TCODK_RIGHT: dx = 1; break;
		case TCODK_LEFT:  dx = -1; break;
		case TCODK_TEXT:
			switch (engine.lastKey.text[0]) { // For single entry non coded keys
			case '<': engine.map->incrementZLevel(-1); break;
			case '>': engine.map->incrementZLevel(1); break;
			}
			break;

		default: break;
		}

		if (dx != 0 || dy != 0) {
			engine.gameStatus = Engine::NEW_TURN;
			position->set(position->x() + dx, position->y() + dy, position->z() );
		}
	}

	Coordinates cameraCoordinates() { return position->coordinates(); }
	int currentZLevel() { return position->z(); }
};