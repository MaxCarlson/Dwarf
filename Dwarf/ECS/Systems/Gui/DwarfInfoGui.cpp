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

void DwarfInfoGui::drawNeeds(std::vector<Needs>& needs, std::vector<std::string> names)
{
	ImGui::Begin("DwarfStats##DwarfInfo", nullptr);

	static const std::string hungerTitle  = "Hunger:  ";
	static const std::string sleepTitle   = "Sleep:   ";
	static const std::string thirstTitle  = "Thrist:  ";
	static const std::string comfortTitle = "Comfort: ";
	static const std::string joyTitle     = "Joy:     ";



	ImGui::Text("Needs");
	for (int i = 0; i < numberShown; ++i)
	{
		ImGui::Text(names[i].c_str());
		
		ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(i / 7.0f, 0.5f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i / 7.0f, 0.9f, 0.9f));



		auto& n = needs[i].needs;
		float sleep   = static_cast<float>(n[static_cast<int>(NeedIdx::SLEEP)].lvl)   / 1000.0f;
		float hunger  = static_cast<float>(n[static_cast<int>(NeedIdx::HUNGER)].lvl)  / 1000.0f;
		float thirst  = static_cast<float>(n[static_cast<int>(NeedIdx::THRIST)].lvl)  / 1000.0f;
		float comfort = static_cast<float>(n[static_cast<int>(NeedIdx::COMFORT)].lvl) / 1000.0f;
		float joy     = static_cast<float>(n[static_cast<int>(NeedIdx::JOY)].lvl)	  / 1000.0f;

		ImVec4 sleepCol = ImColor(IM_COL32(0, 154, 0, 255));

		if (sleep < SleepThreshold::SLEEPY / 1000.0)
			sleepCol = ImColor(IM_COL32(235, 235, 0, 255));

		else if (sleep < SleepThreshold::TIRED / 1000.0)
			sleepCol = ImColor(IM_COL32(204, 110, 0, 255));

		else if (sleep < SleepThreshold::VERY_TIRED / 1000.0)
			sleepCol = ImColor(IM_COL32(204, 0, 0, 255));

		else if (sleep <= SleepThreshold::EXHAUSTED);
			sleepCol = ImColor(IM_COL32(204, 110, 0, 255));
		
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_PlotHistogram, sleepCol);

		ImGui::Text(sleepTitle.c_str()); ImGui::SameLine();
		ImGui::ProgressBar(sleep);

		ImGui::Text(hungerTitle.c_str()); ImGui::SameLine();
		ImGui::ProgressBar(hunger);

		ImGui::Text(thirstTitle.c_str()); ImGui::SameLine();
		ImGui::ProgressBar(thirst);

		ImGui::Text(joyTitle.c_str()); ImGui::SameLine();
		ImGui::ProgressBar(joy);

		ImGui::Text(comfortTitle.c_str()); ImGui::SameLine();
		ImGui::ProgressBar(comfort);
	

		ImGui::PopStyleColor(5);
	}

	ImGui::End();
}
