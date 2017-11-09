#pragma once

#include "../Component.h"

class CameraComponent : public Component
{
public:
	CameraComponent() = default;
	CameraComponent(int width = 50, int height = 50) : width(width), height(height) {};
	// Width and height of the camera box,
	// Ie what do we want to render. Zlevel is held in the camera's position component
	int width, height;
};