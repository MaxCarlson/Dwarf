#include "stdafx.h"
#include "DesignStockpiles.h"
#include "Globals\game_states.h"
#include "Globals\GlobalTerminals.h"
#include "Designations\Designations.h"
#include "Raws\ItemRead.h"
#include "Raws\DefInfo.h"
#include "mouse.h"
#include "KeyDampener.h"
#include "Globals\GlobalWorld.h"
#include "Map\Tile.h"
#include "ECS\Components\Stockpile.h"
#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "Raws\Defs\ItemDefs.h"
#include "Raws\DefInfo.h"
#include "Globals\TexDefines.h"
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

	ImGui::RadioButton("Draw", &designType, DRAW);
	ImGui::RadioButton("Erase", &designType, ERASE);

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

void drawAndConfirm(int type, Coordinates sml, Coordinates lrg, std::function<void(Coordinates, bool)> func)
{
	adjustCoordinatesForLoop(sml, lrg);

	for (int x = sml.x; x <= lrg.x; ++x)
		for (int y = sml.y; y <= lrg.y; ++y)
		{
			const Coordinates pos = { x, y, sml.z };
			const auto idx = getIdx(pos);

			bool possible = false;
			// Don't let just any tile be designated a stockpile
			// Wrap around spurious designations
			// No overwriting old stockpiles ~~ atleast for the moment
			if (!region::flag(pos, region::Flag::CONSTRUCTION) && !region::solid(idx) && region::flag(pos, region::Flag::CAN_STAND_HERE) && region::stockpileId(idx) == 0)
			{
				possible = true;
			}

			func(pos, possible);
		} 
}

void DesignStockpiles::designStockpiles()
{
	ImGui::Text("Select stockpile types");

	static std::vector<bool> selected(defInfo->stockpileNames.size(), 0);

	int i = 0;
	for (const auto& s : defInfo->stockpileNames)
	{
		if (ImGui::Selectable(s.c_str(), selected[i]))
			selected[i] = !selected[i];

		++i;
	}

	if (click != EMPTY_COORDINATES)
	{
		int tCount = 0;
		drawAndConfirm(designType, click, mouse::mousePos, [&tCount](Coordinates xyz, bool possible)
		{
			if (possible)
			{
				++tCount;
				overlayTerm->setChar(xyz.x, xyz.y, vchars{ SQUARE_X_TEX, {0, 235, 0}, {} });
			}
			else
			{
				overlayTerm->setChar(xyz.x, xyz.y, vchars{ SQUARE_X_TEX,{ 255, 0, 0 },{} });
			}
		});

		// Designate
		if (confirm)
		{
			// Set the stockpile squares
			if (designType == DRAW)
			{
				// Set the bitmask for stockpile filters
				std::bitset<NUMBER_OF_ITEM_CATEGORIES> stockSet;
				for (int i = 0; i < defInfo->stockpileNames.size(); ++i)
				{
					if (selected[i])
					{
						const auto set = getStockpileDef(defInfo->stockpileTags[i])->index;
						stockSet.set(set, 1);
					}
				}

				auto newstock = getWorld().createEntity();
				auto sid = newstock.getId().index;

				newstock.addComponent<Stockpile>(Stockpile{ stockSet });
				newstock.addComponent<PositionComponent>(); // Allows rendering
				newstock.addComponent<RenderComponent>();  // ^^

				drawAndConfirm(designType, click, mouse::mousePos, [&sid](Coordinates co, bool possible)
				{
					if (possible)
					{
						region::setStockpileId(getIdx(co), sid);
						region::tile_calc_render(co);
					}
				});

				newstock.activate();
			}

			// Delete stockpile squares selected
			else
			{
				Coordinates sml = click;
				Coordinates lrg = mouse::mousePos;
				adjustCoordinatesForLoop(sml, lrg);

				for(int x = sml.x; x <= lrg.x; ++x)
					for (int y = sml.y; y <= lrg.y; ++y)
					{
						region::setStockpileId(getIdx({ x, y, sml.z }), 0);
						region::tile_calc_render({ x, y, sml.z });
					}
			}

			click = EMPTY_COORDINATES;
			confirm = false;
		}
	}

}
