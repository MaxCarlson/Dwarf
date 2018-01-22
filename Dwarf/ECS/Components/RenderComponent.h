#pragma once

#include "../Component.h"
#include "Drawing\vchar.h"
#include <string>

// Holds info about character to use in render
// as well as colors. 
class RenderComponent : public Component
{
public:
	RenderComponent() = default;
	RenderComponent(vchar ch) : ch(ch) {};

	vchar ch;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(ch);
	}
};

CEREAL_REGISTER_TYPE(RenderComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, RenderComponent);