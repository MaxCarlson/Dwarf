#pragma once

#include "../Systems.h"
#include "../Components/KeyBoardComponent.h"
#include "../Components/PositionComponent.h"
#include "../../Engine.h"
#include "../../Map.h"

// Currently provides just the vertical movement 
// for camera
class CameraSystem : public System<Requires<KeyboardComponent, PositionComponent>> // Possibly require different components in future?
{
public:

	void update()
	{
		auto entities = getEntities();

		for (auto e : entities)
		{
			const auto& key = e.getComponent<KeyboardComponent>();
			auto& co = e.getComponent<PositionComponent>().co;

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
		}
	}

private:
};