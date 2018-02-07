#include "stdafx.h"
#include "DesignBuilding.h"
#include "mouse.h"
#include "Map\Tile.h"
#include "Raws\DefInfo.h"
#include "Raws\Buildings.h"
#include "Globals\game_states.h"
#include "Globals\GlobalTerminals.h"
#include "ECS\Messages\designate_building_message.h"
#include <imgui.h>
#include <imgui_tabs.h>
#include <DwarfRender.h>

void DesignBuilding::init()
{
}

void DesignBuilding::update(const double duration)
{
	ImGui::Begin("Building##Design", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Choose a building and location");

	if (ImGui::Button("Back##BuildingDesignMain"))
	{
		gameState = GameState::PLAYING;
	}

	ImGui::BeginTabBar("Build##TabsDesign");
	ImGui::DrawTabsBackground();

	//static std::vector<std::string> workshops = sepperate buildings into workshops and other catagories once needed

	if (ImGui::AddTab("Workshops"))
	{
		workshopTab();
	}

	if (ImGui::AddTab("Other"))
	{

	}

	ImGui::EndTabBar();
	ImGui::End();
}


void DesignBuilding::drawPossibleBuilding(const std::string &tag)
{
	using namespace mouse;

	std::stringstream ss;
	ss << "Coordiantes: " << mouseX << "," << mouseY << "," << mouseZ;

	ImGui::Text(ss.str().c_str());

	auto* building = getBuilding(tag);

	int X = mouseX;
	int Y = mouseY;

	if (building->width == 3) X -= 1;
	if (building->height == 3) Y -= 1;

	if (X < 0) X = 0; 
	if (Y < 0) Y = 0; 
	if (X + building->width > MAP_WIDTH - 1) X = MAP_WIDTH - 1 - building->width;
	if (Y + building->height > MAP_HEIGHT - 1) Y = MAP_HEIGHT - 1 - building->height;

	// Draw the building and show if it cannot be built there
	int bidx = 0;
	bool possible = true;
	overlayTerm->setAlpha(200);
	for(int x = X; x < building->width + X; ++x)
		for (int y = Y; y < building->height + Y; ++y)
		{
			const int idx = getIdx({ x, y, mouseZ}); // Will have to redo mouse Z once mouse can be on diffrent z layersr

			if (region::solid(idx) || region::getTileType(idx) != region::TileTypes::FLOOR)
			{
				possible = false;
				overlayTerm->setChar(x, y, { 88,{ 255, 0, 0 },{ 255, 0, 0 } });
			}
			else
			{
				overlayTerm->setChar(x, y, { static_cast<uint32_t>(building->charCodes[bidx]), {55, 55, 55}, {55, 55, 55} });
			}

			++bidx;
		}

	if (possible && leftClick)
	{
		const int clickIdx = getIdx({ mouseX, mouseY, mouseZ });
		emit(designate_building_message{ *building, clickIdx });
	}
}

void DesignBuilding::workshopTab()
{
	using namespace mouse;
	ImGui::Text("Build Workshop");

	static bool chooseLoc = false;
	static int selected = 0;

	ImGui::Text("Right click to stop designating");

	ImGui::ListBox("Workshops##ListBuilding", &selected, defInfo->buildingNames);

	if (rightClick)
	{
		chooseLoc = false;
	}

	if (ImGui::Button("Build##WorkshopsBuilding") || chooseLoc)
	{
		chooseLoc = true;
		drawPossibleBuilding(defInfo->buildingTags[selected]);
	}
}
