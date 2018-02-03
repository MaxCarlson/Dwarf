#include "stdafx.h"
#include "DesignDevMode.h"
#include "Globals\game_states.h"
#include "Raws\ItemRead.h"
#include "Raws\DefInfo.h"
#include "Raws\raws.h"
#include <imgui.h>
#include <imgui_tabs.h>

void DesignDevMode::init()
{
}

void chooseLocation(const std::string& tag, const int qty)
{

}

void spawnItem()
{
	static int spawnQty = 1;
	static std::string matTag = "";

	ImGui::Text("Search Items");
	static ImGuiTextFilter itemFilter;
	itemFilter.Draw();

	int itemDefIdx = 0;
	static int itemIdx = 0;
	for (const auto& i : defInfo->itemTags)
	{
		if (itemFilter.PassFilter(i.c_str()))
		{
			ImGui::Text(i.c_str());
			ImGui::SameLine();

			bool selected = itemIdx == itemDefIdx;
			if (ImGui::Checkbox("Select", &selected))
			{
				itemIdx = itemDefIdx;
			}
			ImGui::NextColumn();
		}
		++itemDefIdx;
	}

	ImGui::Begin("Material", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Search Materials");

	static ImGuiTextFilter matFilter;
	matFilter.Draw();

	int matDefIdx = 0;
	static int matIdx = 0;
	for (const auto& m : defInfo->materialTags)
	{
		if (matFilter.PassFilter(m.c_str()))
		{
			ImGui::Text(m.c_str());
			ImGui::SameLine();
			bool selected = matIdx == matDefIdx;
			if (ImGui::Checkbox("Select", &selected))
			{
				matIdx = matDefIdx;
			}

			ImGui::NextColumn();
		}
		++matDefIdx;
	}
	ImGui::End();

	ImGui::Begin("Qty", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	static int qty = 1;
	ImGui::SliderInt("Qty", &qty, 1, 100);

	ImGui::End();
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

	ImGui::EndTabBar();
	ImGui::End();
}
