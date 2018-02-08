#include "stdafx.h"
#include "DesignDevMode.h"
#include "Globals\game_states.h"
#include "Raws\ItemRead.h"
#include "Raws\DefInfo.h"
#include "Raws\Materials.h"
#include "Raws\raws.h"
#include "Globals\Camera.h"
#include "Coordinates.h"
#include "ECS\Messages\update_all_maps_message.h"
#include "KeyDampener.h"
#include "mouse.h"
#include "Map\Tile.h"
#include <imgui.h>
#include <imgui_tabs.h>
#include <DwarfRender.h>

void DesignDevMode::init()
{
}

bool DesignDevMode::chooseLocation(const std::string& itemTag, const std::string& matTag, const int qty)
{
	using namespace mouse;
	int mx = mouseX;
	int my = mouseY;

	mx += camera.offsetX;
	my += camera.offsetY;

	std::string coor = "Coordinates: x = " + std::to_string(mx) + " y = " + std::to_string(my) + " z = " + std::to_string(mouseZ);

	ImGui::Text(coor.c_str());
	if (mouse::leftClick)
	{
		spawnItemOnGround(itemTag, getMaterialIdx(matTag), { mx, my, mouseZ }, SpawnColor::MATERIAL_COLOR);

		emit(update_all_maps_message{});
		return true;
	}
	return false;
}

void DesignDevMode::spawnItem()
{
	static bool selectingSpawnLocation = false;
	static int spawnQty = 1;
	static std::string matTag = "";

	// Pick Item spawn Quantity
	static int qty = 1;
	ImGui::SliderInt("Qty", &qty, 1, 100);

	// Filter and list Items
	ImGui::Text("Search Items");
	static ImGuiTextFilter itemFilter;
	itemFilter.Draw();

	std::vector<std::string> itemsPassedFilter;
	for (const auto& i : defInfo->itemTags)
		if (itemFilter.PassFilter(i.c_str()))
			itemsPassedFilter.emplace_back(i);

	static int itemIdx = 0;
	ImGui::ListBox("Items##Dev", &itemIdx, itemsPassedFilter);

	ImGui::Begin("Material", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	// Filter and list Materials
	ImGui::Text("Search Materials");

	static ImGuiTextFilter matFilter;
	matFilter.Draw();

	std::vector<std::string> matsPassedFilter;
	for (const auto& m : defInfo->materialTags)
		if (matFilter.PassFilter(m.c_str()))
			matsPassedFilter.emplace_back(m);

	static int matIdx = 0;
	ImGui::ListBox("Materials##Dev", &matIdx, matsPassedFilter);

	ImGui::End();

	if (ImGui::Button("Select Spawn Location##Dev") || selectingSpawnLocation)
	{
		selectingSpawnLocation = !chooseLocation(itemsPassedFilter.at(itemIdx), matsPassedFilter.at(matIdx), qty);

		if (!selectingSpawnLocation)
		{
			//itemFilter.Clear();
			//matFilter.Clear();
			//itemIdx = 0;
			//matIdx = 0;
		}
	}
}

void printTileInfo()
{
	using namespace region;
	std::stringstream ss;
	const auto co = mouse::mousePos;
	const auto idx = getIdx(co);

	ss << "Solid: " << region::solid(idx) << "\n";
	ss << "Flags: " << "Stand: " << flag(co, Flag::CAN_STAND_HERE) << "\n"; // Eventually add in naming instead of using the number if it's worth the time

	ss << "TileType: " << getTileType(idx);

	ImGui::Text(ss.str().c_str());
}

void DesignDevMode::update(const double duration)
{
	ImGui::Begin("Dev Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::Button("Close"))
	{
		gameState = GameState::PLAYING;
	}

	ImGui::BeginTabBar("Dev Control");
	ImGui::DrawTabsBackground();

	if (ImGui::AddTab("Item Spawning"))
	{
		spawnItem();
	}

	if (ImGui::AddTab("Delete##Dev"))
	{

	}

	if(ImGui::AddTab("TileInfo##Dev"))
	{
		printTileInfo();
	}

	ImGui::EndTabBar();
	ImGui::End();
}
