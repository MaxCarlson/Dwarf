#pragma once

#include "../Systems.h"
#include "../Components/PositionComponent.h"
#include "../Components/RenderComponent.h"
#include "../BearLibTerminal.h"
#include "../Components/CameraComponent.h"

class RenderSystem : public System<Requires<PositionComponent, RenderComponent>>
{
public:
	RenderSystem() = default;
	RenderSystem(Map * map) : map(map) {};
	~RenderSystem() { delete map; }

	// Render system holds a pointer to the map we're 
	// rendering. Map also holds the MapRender object which holds camera info
	Map * map;

	void update()
	{
		auto entities = getEntities();

		int offsetX = map->mapRenderer->offsetX;
		int offsetY = map->mapRenderer->offsetY;

		for (auto e : entities)
		{
			auto co = e.getComponent<PositionComponent>().co;

			// Adjust copy of position coordinates to be aligned with camera
			// coordinates
			co.x -= offsetX;
			co.y -= offsetY;

			// If the camera z level matches our entities
			// zLevel, render it! 
			// (Eventually add opacity to non obscured Entities below camera z Level)
			if (isInCameraRange(co)) {
				const auto& rend = e.getComponent<RenderComponent>();

				// Libtcod
				TCODConsole::root->setCharBackground(co.x, co.y, rend.backColor);
				TCODConsole::root->setCharForeground(co.x, co.y, rend.foreColor);
				TCODConsole::root->setChar(co.x, co.y, rend.ch);				  // Look at other functions like root->putChar, etc!!!!

				// BearslibTerminal
				terminal_put(co.x, co.y, 0xE200 + rend.ch);
			}
		}
	}

	// Is the object we're trying to render 
	// within view of the camera ?
	inline bool isInCameraRange(Coordinates co)
	{
		if(map->mapRenderer->currentZLevel != co.z
			|| co.x < 0
			|| co.y < 0
			)
			return false;

		return true;
	}

	void initialize() {};

	void onEntityAdd(Entity & entity) {};

	void onEntityRemoval(Entity & entity) {};
private:
};