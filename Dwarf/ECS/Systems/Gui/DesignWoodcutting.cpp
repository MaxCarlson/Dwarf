#include "stdafx.h"
#include "DesignWoodcutting.h"
#include "Globals\Camera.h"
#include "Globals\game_states.h"
#include "Globals\GlobalTerminals.h"
#include "Map\Tile.h"
#include "ECS\Messages\designation_message.h"
#include "mouse.h"
#include "Coordinates.h"
#include "ECS\World.h"
#include <imgui.h>
#include <DwarfRender.h>

using namespace mouse;

void DesignWoodcutting::init()
{
}

static bool clicked = false;
static Coordinates firstClick;

void DesignWoodcutting::drawCuttingArea(bool first)
{
	if(first)
		firstClick = mousePos;

	// Grab the second click. There has to be a less ugly way to do these  two clicks?
	if (leftClick && !first)
	{
		clicked = false;
		emit(designation_message{ { getIdx(firstClick), getIdx(mousePos) }, designation_message::CUT_TREES });
	}

	// Drop out if right click
	if (rightClick)
	{
		clicked = false;
		return;
	}

	int x1 = mouseX;
	int x2 = firstClick.x;
	int y1 = mouseY;
	int y2 = firstClick.y;

	// Flip mouse coordinates so we can
	// iterate through the fieled between properly
	if (x1 > x2)
	{
		int tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	if (y1 > y2)
	{
		int tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	
	// Draw highlighted area where chopping will be done if clicked
	int treeCount = 0;
	overlayTerm->setAlpha(115);
	for(int x = x1; x <= x2; ++x)
		for (int y = y1; y <= y2; ++y)
		{
			overlayTerm->setChar(x, y, { 88, { 255, 255, 0 }, { 0, 0, 0 } });

			if (region::treeId(getIdx({ x, y, mouseZ })))
			{
				overlayTerm->setAlpha(225);
				overlayTerm->setChar(x, y, { 88,{ 255, 0, 0 }, { 255, 0, 0 } });
				overlayTerm->setAlpha(115);
				++treeCount;
			}
		}

	// Print out info about selected area
	std::stringstream ss;
	ss << "Coordinate 1: " << x1 << "," << y1 << "," << mouseZ << " Coordinate 2: " << x2 << "," << y2 << "," << mouseZ;
	ImGui::Text(ss.str().c_str()); 
	ss.str(std::string());
	ss << "Tree Count: " << treeCount;
	ImGui::Text(ss.str().c_str());

}

void DesignWoodcutting::update(const double duration)
{
	// Show # logs in stockpiles eventually probably store item info in stockpiles globally,
	// perhaps a unfrequent updating system

	ImGui::Begin("Woodcutting", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
	
	if (ImGui::Button("Back##Woodcutting"))
	{
		gameState = GameState::PLAYING;
	}

	ImGui::Text("Click anywhere on map and click again");
	ImGui::Text("to designate area for chopping");
	ImGui::Text("Right click to cancel");

	static int autoChopCount = 0;

	// Add in an auto-chop feature
	ImGui::Text("Auto designate trees for cutting");
	ImGui::SliderInt("Auto-Chop: ", &autoChopCount, 0, 50);

	if (ImGui::Button("Auto-Chop##do"))
	{

	}

	// Do highlight woodcutting designations
	if (leftClick || clicked)
	{
		bool first = false;
		if (!clicked)
			first = true;

		clicked = true;

		drawCuttingArea(first);
	}

	ImGui::End();

	// Add in chop till amount of wood in stockpiles/cut
}
