#pragma once

#include "../Component.h"

class CameraComponent : public Component
{
public:
	CameraComponent() = default;
	CameraComponent(int width = 50, int height = 50, int currentCellSize = 16)
		         : width(width), height(height), currentCellSize(currentCellSize) {};

	// Width and height of the camera box,
	// Ie what do we want to render. Zlevel is held in the camera's position component
	int width;
	int height;

	// Current size of the length/width in pixels
	// of tiles
	int currentCellSize;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::base_class<Component>(this), width, height, currentCellSize);
	}
};