#include "stdafx.h"
#include "DesignBuilding.h"
#include "mouse.h"
#include "Map\Tile.h"
#include "Raws\DefInfo.h"
#include "Raws\Buildings.h"
#include "Globals\game_states.h"
#include "Globals\GlobalTerminals.h"
#include "ECS\Messages\designate_building_message.h"
#include "Designations.h"
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
			const Coordinates cox = { x, y, mouseZ };
			const int idx = getIdx(cox); // Will have to redo mouse Z once mouse can be on diffrent z layers?
			
			bool squareDesignated = false;

			std::for_each(designations->buildings.begin(), designations->buildings.end(), // Should building designations be a map?
				[&cox, &squareDesignated](const building_designation& bd)
			{
				if (bd.co.z == cox.z 
					&& (bd.co.x >= cox.x && bd.co.x < cox.x + bd.width)
					&& (bd.co.y >= cox.y && bd.co.y < cox.y + bd.height))
				{
					squareDesignated = true;
				}
			});

			if (squareDesignated || region::solid(idx) || region::getTileType(idx) != region::TileTypes::FLOOR || region::flag(cox, region::Flag::CONSTRUCTION))
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
		designateBuilding(tag, clickIdx);
	}
}

void DesignBuilding::workshopTab()
{
	using namespace mouse;
	ImGui::Text("Build Workshop");

	static bool chooseLoc = false;
	static int selected = 0;

	ImGui::Text("Right click to stop designating");

	ImGui::ListBox("Workshops##ListBuilding", &selected, defInfo->buildingNames); // Break buildings into catagories and create another bitset to manage their gui

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

#include "ECS\Components\PositionComponent.h"
#include "ECS\Systems\helpers\ItemHelper.h"
#include "ECS\Components\Building.h"
#include "ECS\Systems\EntityPositionCache.h"

void DesignBuilding::designateBuilding(const std::string & tag, const int idx)
{
	building_designation designation;

	auto buildingDef = getBuilding(tag);

	if (buildingDef == nullptr)
		return;

	designation.tag = tag;
	designation.name = buildingDef->name;
	designation.co = idxToCo(idx);
	designation.width = buildingDef->width;
	designation.height = buildingDef->height;
	designation.components = buildingDef->components;


	// Find building components
	for (const auto& comp : buildingDef->components)
	{
		// Set correct quanitity of components needed
		for (int i = 0; i < comp.quantity; ++i)
		{
			const auto compId = itemHelper.claim_item_by_reaction_inp(comp);

			std::cout << "Component [" << comp.tag << "] - Id " << compId << "\n";

			designation.componentIds.push_back(std::make_pair(compId, false));
		}
	}

	auto building = getWorld().createEntity();

	building.addComponent<PositionComponent>(designation.co);
	building.addComponent<Building>(Building{ tag, buildingDef->width, buildingDef->height, false, buildingDef->provides, buildingDef->charCodes });

	auto& bbb = building.getComponent<Building>();

	designation.entity_id = building.getId().index;

	// Adjust center for 3 tile buildings
	int sx = designation.co.x;
	int sy = designation.co.y;
	if (designation.width == 3) --sx;
	if (designation.height == 3) --sy;

	for (int x = sx; x < sx + designation.width; ++x)
		for (int y = sy; y < sy + designation.height; ++y)
		{
			const Coordinates co = { x, y, designation.co.z };
			region::setFlag(co, region::Flag::CONSTRUCTION);

			// Id's must be manually deleted later
			positionCache->addNode({ co, designation.entity_id });
		}

	designations->buildings.push_back(designation);
}
