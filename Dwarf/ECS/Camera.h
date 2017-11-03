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
	CameraComponent() {};

	void init() override
	{
		position = &entity->getComponent<PositionComponent>();
	}
};