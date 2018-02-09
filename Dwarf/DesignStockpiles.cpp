#include "stdafx.h"
#include "DesignStockpiles.h"
#include "Globals\game_states.h"
#include "Globals\GlobalTerminals.h"
#include "Designations.h"
#include "Raws\ItemRead.h"
#include "Raws\DefInfo.h"
#include "mouse.h"
#include "KeyDampener.h"
#include "Globals\GlobalWorld.h"
#include "Map\Tile.h"
#include "ECS\Components\Stockpile.h"
#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\RenderComponent.h"
#include <imgui.h>
#include <imgui_tabs.h>
#include <DwarfRender.h>


void DesignStockpiles::update(const double duration)
{
	ImGui::Begin("Stockpiles##Design", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	if (ImGui::Button("Close##StockpilesDesign"))
	{
		confirm = false;
		click = EMPTY_COORDINATES;
		gameState = GameState::PLAYING;
	}


	ImGui::Text("Design Stockpiles");
	ImGui::Text("Right click to stop designating");

	if (mouse::rightClick)
	{
		click = EMPTY_COORDINATES;
	}

	// Set us to either drawing the highlighted area or confirming the command
	if (mouse::leftClick && !ImGui::IsMouseHoveringWindow())
	{
		if (click == EMPTY_COORDINATES)
			click = mouse::mousePos;
		else
			confirm = true;
	}

	designStockpiles();

	ImGui::End();
}

void drawAnConfirm(int type, Coordinates sml, Coordinates lrg, const std::bitset<64> &catagories, std::function<void(Coordinates, bool)> func)
{
	adjustCoordinatesForLoop(sml, lrg);

	auto newstock = world.createEntity();
	auto sid = newstock.getId().index;

	newstock.addComponent<Stockpile>(Stockpile{ catagories });
	newstock.addComponent<PositionComponent>(); // Allows rendering
	newstock.addComponent<RenderComponent>();  // ^^

	for (int x = sml.x; x <= lrg.x; ++x)
		for (int y = sml.y; y <= lrg.y; ++y)
		{
			const Coordinates pos = { x, y, sml.z };
			const auto idx = getIdx(pos);

			// Don't let just any tile be designated a stockpile
			// Wrap around spurious designations
			// No overwriting old stockpiles ~~ atleast for the moment
			if (!region::flag(pos, region::Flag::CONSTRUCTION) && !region::solid(idx) && region::flag(pos, region::Flag::CAN_STAND_HERE) && region::stockpileId(idx) == 0)
			{
				region::setStockpileId(idx, sid);
			}
		}

	newstock.activate();
}

void DesignStockpiles::designStockpiles()
{
	static int stockSelected = 0;

	ImGui::ListBox("List of Stockpiles", &stockSelected, defInfo->stockpileNames); 

	if (click != EMPTY_COORDINATES)
	{

	}

	if (confirm)
	{

	}
}
