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

	const int terminalCodes[9] = { 0xE000, 0xE100, 0xE200, 0xE300, 0xE400, 0xE500, 0xE600, 0xE700, 0xE800 };

	void init();

	void update();

	void updateRender();

	/*
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
	*/

private:

	bool renderChanged = true;
};