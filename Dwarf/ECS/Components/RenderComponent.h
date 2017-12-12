#pragma once

#include "../Component.h"

#include <string>

// Holds info about character to use in render
// as well as colors. 
class RenderComponent : public Component
{
public:
	RenderComponent() = default;
	RenderComponent(int ch, int terminalCode, std::string colorStr)
		: ch(ch), terminalCode(terminalCode), colorStr(colorStr) {};

	// Character code representation
	int ch;

	// Code to use to index which image
	// we want to draw from
	int terminalCode;

	// Possibly create a color object to house some simple color opperations?
	// Color string for BLT renderer
	// Will convert to const char* when actually rendering
	std::string colorStr;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::base_class<Component>(this), ch, terminalCode, colorStr);
	}
};

CEREAL_REGISTER_TYPE(RenderComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, RenderComponent);