#include "stdafx.h"
#include "Globals\Camera.h"
#include "CameraSystem.h"
#include "Map\Tile.h"
#include "KeyDampener.h"
#include <DwarfRender.h>
#include <imgui.h>

void CameraSystem::init()
{
}

void CameraSystem::update(const double duration)
{
	using namespace keys;

	if (isKeyDown(sf::Keyboard::Right, true))
	{
		++camera.x;
		if (camera.x > MAP_WIDTH - 1)
			camera.x = MAP_WIDTH - 1;
	}

	if (isKeyDown(sf::Keyboard::Left, true))
	{
		--camera.x;
		if (camera.x < 0)
			camera.x = 0;
	}

	if (isKeyDown(sf::Keyboard::Up, true))
	{
		--camera.y;
		if (camera.y < 0)
			camera.y = 0;
	}

	if (isKeyDown(sf::Keyboard::Down, true))
	{
		++camera.y;
		if (camera.y > MAP_HEIGHT - 1)
			camera.y = MAP_HEIGHT - 1;
	}

	if (isKeyDown(sf::Keyboard::Comma, true))
	{
		--camera.z;
		if (camera.z < 1)
			camera.z = 1;
	}

	if (isKeyDown(sf::Keyboard::Period, true))
	{
		++camera.z;
		if (camera.z > MAP_DEPTH - 1)
			camera.z = MAP_DEPTH - 1;
	}
}
