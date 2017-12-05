#pragma once

#include "../Component.h"
#include "RenderComponent.h"
class Entity;
class RenderComponent;

struct ItemStored : public Component
{
	ItemStored() = default;
	ItemStored(std::size_t eid) : eid(eid) {}

	// Refrence to the Entity of the item itself
	std::size_t eid;

	// Copy of the stored Items renderer
	// when it's stored
	RenderComponent rend;
};