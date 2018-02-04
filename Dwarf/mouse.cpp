#include "stdafx.h"
#include "mouse.h"
#include "Globals\Camera.h"
#include "Map\Tile.h"
#include <imgui.h>
#include <DwarfRender.h>

namespace mouse
{
	int mouseX;
	int mouseY;
	int mouseZ;

	bool leftClick;
	bool rightClick;
	bool middleClick;
	bool wheelUp;
	bool wheelDown;
	int mousePX;
	int mousePY;

	Coordinates mousePos;

	void readSFMLPos()
	{
		auto mxy = dfr::getMousePosition();

		mouseX = mxy.first - camera.offsetX;
		mouseY = mxy.second - camera.offsetY;
		mouseZ = camera.z;

		if (mouseX < 0) mouseX = 0;
		if (mouseY < 0) mouseY = 0;
		if (mouseX > MAP_WIDTH - 1) mouseX = MAP_WIDTH - 1;
		if (mouseY > MAP_HEIGHT - 1) mouseY = MAP_HEIGHT - 1;

		mousePos = { mouseX, mouseY, mouseZ };
	}

	void readMouse()
	{
		ImGuiIO &io = ImGui::GetIO();

		readSFMLPos();

		auto mpix = dfr::getMousePositionPx();
		mousePX = mpix.first;
		mousePY = mpix.second;

		leftClick = io.MouseDown[0];
		rightClick = io.MouseDown[1];
		middleClick = io.MouseDown[2];

		auto wheel = io.MouseWheel;

		wheelUp = false;
		wheelDown = false;

		if (wheel > 0)
			wheelDown = true;
		if (wheel < 0)
			wheelUp = true;
	}
}