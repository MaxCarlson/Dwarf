#pragma once

#include "../Component.h"
#include "RenderComponent.h"
class Entity;
class RenderComponent;

struct ItemStored : public Component
{
	ItemStored() = default;
	ItemStored(std::size_t eid) : eid(eid) {}

	// Refrence to the Entity storing the tool
	std::size_t eid;

	// Copy of the stored Items renderer
	// when it's stored
	RenderComponent rend;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::base_class<Component>(this), eid, rend);
	}
};