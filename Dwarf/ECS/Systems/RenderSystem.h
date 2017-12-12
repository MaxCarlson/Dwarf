#pragma once

#include "../Systems.h"
#include "../Components/PositionComponent.h"
#include "../Components/RenderComponent.h"
#include "../BearLibTerminal.h"


// Render all entities that are within camera fov/zlevel
// and are visible
class RenderSystem : public System<Requires<PositionComponent, RenderComponent>>
{
public:
	RenderSystem() = default;

	void update()
	{
		const auto& entities = getEntities();

		int offsetX = engine->mapRenderer->offsetX; // Use gui offset if rendering becomes a resource drain!!
		int offsetY = engine->mapRenderer->offsetY;
		
		terminal_color("default");

		for (const auto& e : entities)
		{
			auto co = e.getComponent<PositionComponent>().co;

			// Adjust copy of position coordinates to be aligned with camera
			// coordinates
			co.x -= offsetX;
			co.y -= offsetY;

			// If the camera z level matches our entities
			// zLevel, render it! 
			// (Eventually add opacity to non obscured Entities below camera z Level)
			if (isInCameraRange(co))
			{
				const auto& rend = e.getComponent<RenderComponent>();				  

				const char* cstr = rend.colorStr.c_str();

				// Switch terminal color to item color
				terminal_color(cstr);

				// Draw tile image, from tile set associated with rend.terminalcode
				// with location in tileset being rend.ch
				terminal_put(co.x, co.y, rend.terminalCode + rend.ch);
			}
		}
	}

	// Is the object we're trying to render 
	// within view of the camera ? // Performance wise, is it worth adding more checks such as co.x < 0 at the risk of branching?? Revisit
	inline bool isInCameraRange(Coordinates co)
	{
		if(engine->mapRenderer->currentZLevel != co.z
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