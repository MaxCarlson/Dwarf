#pragma once

#include "../Component.h"
#include "RenderComponent.h"
class RenderComponent;

struct ItemCarried : public Component
{
	ItemCarried() = default;
	ItemCarried(std::size_t eid) : eid(eid) {}

	// Refrence to the Entity storing the item
	std::size_t eid;

	// Copy of the stored Items renderer
	// when it's stored
	RenderComponent rend;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(eid, rend);
	}
};

CEREAL_REGISTER_TYPE(ItemCarried);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ItemCarried);