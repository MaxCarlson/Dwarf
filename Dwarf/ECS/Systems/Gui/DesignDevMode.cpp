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

	static int itemIdx = 0;
	static int matIdx = 0;

	int itemDefIdx = 0;
	for (const auto& i : defInfo->itemTags)
	{
		ImGui::Text(i.c_str());
		ImGui::SameLine();

		bool selected = itemIdx == itemDefIdx;
		if (ImGui::Checkbox("Select", &selected))
		{

		}

		ImGui::NextColumn();
		++itemDefIdx;
	}
}

void DesignDevMode::update(const double duration)
{
	ImGui::Begin("Dev Controls", nullptr, ImGuiWindowFlags_NoCollapse);

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
