#pragma once
#include "include\libtcod.hpp"
#include "Entity.h"
#include "Tile.h"

// Holds the map index for the particular componenet
enum CompID
{
	COMPID_RENDER,
	COMPID_PLAYER
};

class Component : GameObject
{
public:
	Component(CompID compId) : compID(compId) {};
	CompID compID;
};

class RenderComponent : public Component
{
public:
	RenderComponent(CompID compId, TCODColor col, Coordinates cor) : Component(compId), col(col), cor(cor) {};
	TCODColor & col;
	Coordinates & cor;
};

class Player : public Component
{
	Player(CompID compId, TCODColor col, Coordinates cor) : Component(compId), col(col), cor(cor) {};
	TCODColor & col;
	Coordinates & cor;
};