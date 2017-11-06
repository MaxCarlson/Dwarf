#pragma once

#include "../Systems.h"
#include "../Components/PositionComponent.h"
#include "../Components/RenderComponent.h"

class RenderSystem : public System<Requires<PositionComponent, RenderComponent>>
{
public:
	RenderSystem() = default;
	RenderSystem(Entity& mCamera) : masterCamera(mCamera) {};

	// Holds a refrence to the master camera
	// so we know when a tile is out of frame/zLevel 
	// (and don't need to render it!)
	Entity& masterCamera;             /// Add some zlevel caching and turn of render component for when the camera is staying still for performace benifit???

	void update()
	{
		auto entities = getEntities();

		for (auto e : entities)
		{
			const auto& co = e.getComponent<PositionComponent>().co;
			const auto& rend = e.getComponent<RenderComponent>();
			TCODConsole::root->setCharBackground(co.x, co.y, rend.backColor);
		}
	}
};