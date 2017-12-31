#pragma once

#include "../Component.h"
#include "RenderComponent.h"
class Entity;
class RenderComponent;


struct ItemStored : public Component
{
	ItemStored() = default;
	ItemStored(std::size_t storedIn) : storedIn(storedIn) {}

	// Refrence to the Entity storing the item
	std::size_t storedIn;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(storedIn);
	}
};

CEREAL_REGISTER_TYPE(ItemStored);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ItemStored);
