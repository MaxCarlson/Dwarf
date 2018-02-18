#include "stdafx.h"
#include "DwarfInfoGui.h"
#include "mouse.h"
#include "KeyDampener.h"
#include "ECS\Components\Sentients\Needs.h"
#include "ECS\Components\Sentients\Stats.h"
#include "Globals\game_states.h"
#include <imgui.h>
#include <imgui_tabs.h>

void DwarfInfoGui::init()
{
}

void DwarfInfoGui::update(const double duration)
{
	ImGui::Begin("Dwarf Info", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::Button("Close##DwarfInfo"))
	{
		designState = DesignStates::ARCHITECURE; // Doesn't matter which one as long as it's an auto-pausing system
	}

	// Find all work-eligible entities
	int i = 1;
	std::vector<Needs> dNeeds;
	std::vector<Stats> dStats;
	std::vector<std::string> dNames;
	for (const auto& e : getEntities()) // TODO: Once real names are added in add a filter so we can search
	{
		static const std::string n = "Dwarf ";
		dNames.emplace_back(n + std::to_string(i));
		dNeeds.emplace_back(e.getComponent<Needs>());
		dStats.emplace_back(e.getComponent<Stats>());
		++i;
	}

	if (numberShown > dNames.size() - 1)
		numberShown = dNames.size() - 1;

	ImGui::Text("How many Dwarves info to show");
	ImGui::SliderInt("Show Number:", &numberShown, 1, dNames.size());

	ImGui::BeginTabBar("Info##Info");

	if (ImGui::AddTab("Needs"))
	{
		drawNeeds(dNeeds, dNames);
	}

	if (ImGui::AddTab("Stats"))
	{

	}

	if (ImGui::AddTab("Inventory"))
	{

	}

	ImGui::EndTabBar();
	ImGui::End();
}

inline void determineNeedColor(const float need, ImVec4 &needCol)
{
	if (need <= 0.10f)
		needCol = ImColor(IM_COL32(204, 110, 0, 255));

	else if (need < 0.20f)
		needCol = ImColor(IM_COL32(204, 0, 0, 255));

	else if (need < 0.35f)
		needCol = ImColor(IM_COL32(204, 110, 0, 255));

	else if (need < 0.50f)
		needCol = ImColor(IM_COL32(235, 235, 0, 255));
}

inline void drawNeedProgress(const std::string& title, const float need)
{
	static const ImVec4 greenBar = ImColor(IM_COL32(0, 154, 0, 255));
	ImVec4 col = greenBar;

	determineNeedColor(need, col);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_PlotHistogram, col);

	ImGui::Text(title.c_str()); ImGui::SameLine();
	ImGui::ProgressBar(need);
	ImGui::PopStyleColor();
}

void DwarfInfoGui::drawNeeds(std::vector<Needs>& needs, std::vector<std::string> names)
{
	ImGui::Begin("DwarfStats##DwarfInfo", nullptr);

	static const std::string thirstTitle = "Thrist:   ";
	static const std::string hungerTitle  = "Hunger:  ";
	static const std::string sleepTitle   = "Sleep:   ";
	static const std::string comfortTitle = "Comfort: ";
	static const std::string joyTitle     = "Joy:     ";

	static const std::vector<std::string> needNames = { thirstTitle, hungerTitle, sleepTitle, comfortTitle, joyTitle };

	ImGui::Text("Needs");
	for (int i = 0; i < numberShown; ++i)
	{
		ImGui::Text(names[i].c_str());
		
		ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(7.0f, 0.5f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i / 7.0f, 0.9f, 0.9f));

		// Draw need bars
		int nIdx = 0;
		for (auto& n : needs[i].needs)
		{
			auto needLvl = static_cast<float>(n.lvl) / 1000.0f;
			drawNeedProgress(needNames[nIdx], needLvl);
			++nIdx;
		}

		ImGui::PopStyleColor(4);
	}

	ImGui::End();
}
