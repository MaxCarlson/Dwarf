#pragma once

#include "../Systems.h"
#include "../Components/PositionComponent.h"
#include "../Components/RenderComponent.h"

class RenderSystem : public System<Requires<PositionComponent, RenderComponent>>
{
public:
	RenderSystem() = default;
	RenderSystem(Coordinates* mCameraPos) : mCameraPos(mCameraPos) {};
	~RenderSystem() { delete mCameraPos; }

	// Holds a refrence to the master camera coordinates
	// so we know when a tile is out of frame/zLevel 
	// (and don't need to render it!)
	Coordinates * mCameraPos;      /// Add some zlevel caching (possibly entity caching too?) and turn off render component for when the camera is staying still for performace benifit???

	void update()
	{
		auto entities = getEntities();

		for (auto e : entities)
		{
			const auto& co = e.getComponent<PositionComponent>().co;

			// If the camera z level matches our entities
			// zLevel, render it! 
			// (Eventually add opacity to non obscured Entities below camera z Level)
			if (co.z == mCameraPos->z) {
				const auto& rend = e.getComponent<RenderComponent>();
				TCODConsole::root->setCharBackground(co.x, co.y, rend.backColor);
				TCODConsole::root->setCharForeground(co.x, co.y, rend.foreColor);
				TCODConsole::root->setChar(co.x, co.y, rend.ch);				  // Look at other functions like root->putChar, etc!!!!
			}
		}
	}

	void initialize() {};

	void onEntityAdd(Entity & entity) {};

	void onEntityRemoval(Entity & entity) {};
private:
};