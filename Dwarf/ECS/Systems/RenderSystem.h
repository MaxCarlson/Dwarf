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
	RenderSystem(Coordinates* mCameraPos, CameraComponent * mCameraComp) : mCameraPos(mCameraPos), mCameraComp(mCameraComp) {};
	~RenderSystem() { delete mCameraPos; }

	// Holds a pointer to the master camera coordinates
	// so we know when a tile is out of frame/zLevel 
	// (and don't need to render it!)
	Coordinates * mCameraPos;      /// Add some zlevel caching (possibly entity caching too?) and turn off render component for when the camera is staying still for performace benifit???

	// Holds a pointer to the camera componenet 
	// of main camera. Holds info such as width
	// and height we need to render
	CameraComponent * mCameraComp;

	void update()
	{
		auto entities = getEntities();

		for (auto e : entities)
		{
			const auto& co = e.getComponent<PositionComponent>().co;

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
		if (co.z != mCameraPos->z
			|| co.x < mCameraPos->x - mCameraComp->width / 2 - 1		/// Revisit these - and + 's to be sure that's the way it should work once camera zoom is working
			|| co.x > mCameraPos->x + mCameraComp->width / 2 + 1
			|| co.y < mCameraPos->y - mCameraComp->height / 2 - 1
			|| co.y > mCameraPos->y + mCameraComp->height / 2 + 1)
			return false;

		return true;
	}

	void initialize() {};

	void onEntityAdd(Entity & entity) {};

	void onEntityRemoval(Entity & entity) {};
private:
};