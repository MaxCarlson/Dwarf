#include "stdafx.h"
#include "Globals\Camera.h"
#include "CameraSystem.h"
#include "Map\Tile.h"
#include <DwarfRender.h>
#include <imgui.h>

void CameraSystem::init()
{
}

void CameraSystem::update(const double duration)
{
	ImGuiIO& io = ImGui::GetIO();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		++camera.x;
		if (camera.x > MAP_WIDTH - 1)
			camera.x = MAP_WIDTH - 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		--camera.x;
		if (camera.x < 0)
			camera.x = 0;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		--camera.y;
		if (camera.y < 0)
			camera.y = 0;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		++camera.y;
		if (camera.y > MAP_HEIGHT - 1)
			camera.y = MAP_HEIGHT - 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma))
	{
		--camera.z;
		if (camera.z < 1)
			camera.z = 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period))
	{
		++camera.z;
		if (camera.z > MAP_DEPTH - 1)
			camera.z = MAP_DEPTH - 1;
	}
}
