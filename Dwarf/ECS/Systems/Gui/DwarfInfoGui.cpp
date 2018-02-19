#include "stdafx.h"
#include "DwarfInfoGui.h"
#include "mouse.h"
#include "KeyDampener.h"
#include "ECS\Components\Sentients\Needs.h"
#include "ECS\Components\Sentients\Stats.h"
#include "Globals\game_states.h"
#include "Raws\SkillReader.h"
#include <imgui.h>
#include <imgui_tabs.h>

#include "Globals\GlobalWorld.h"


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

	static ImGuiTextFilter filter;

	// Find all work-eligible entities
	int i = 1;
	std::vector<std::string> dNames;
	std::vector<Entity> entsPassedFilter; // TODO: Add in a compare dwarves feature TODO: add into compare a visual reprsentation of differences
	for (const auto& e : getEntities())
	{
		static const std::string n = "Dwarf ";  // TODO: Add in real names

		if (!filter.PassFilter(n.c_str()))
			continue;

		dNames.emplace_back(n + std::to_string(i));
		entsPassedFilter.emplace_back(e);
		++i;
	}

	if (dwarfSelected > dNames.size() - 1)
		dwarfSelected = dNames.size() - 1;

	ImGui::ListBox("Dwarf Names##DwarfInfo", &dwarfSelected, dNames);

	ImGui::Begin("DwarfInfo##DwarfSelctedInfo", nullptr);

	ImGui::Columns(3);
	ImGui::Separator();

	const auto& dEntSelected = entsPassedFilter[dwarfSelected];

	drawNeeds(dNames[dwarfSelected], dEntSelected);

	ImGui::NextColumn();

	drawStats(dEntSelected);

	ImGui::NextColumn();

	drawInventory(dEntSelected);

	ImGui::NextColumn();
	
	ImGui::End();

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

void DwarfInfoGui::drawNeeds(const std::string &name, const Entity& e)
{

	static const std::string thirstTitle  = "Thrist:  ";
	static const std::string hungerTitle  = "Hunger:  ";
	static const std::string sleepTitle   = "Sleep:   ";
	static const std::string comfortTitle = "Comfort: ";
	static const std::string joyTitle     = "Joy:     ";

	static const std::vector<std::string> needNames = { thirstTitle, hungerTitle, sleepTitle, comfortTitle, joyTitle };

	ImGui::Text("Needs");

	ImGui::Text(name.c_str());
		
	ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(7.0f, 0.5f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(7.0f, 0.9f, 0.9f));

	// Draw need bars
	int nIdx = 0;
	auto& needs = e.getComponent<Needs>();

	for (auto& n : needs.needs)
	{
		auto needLvl = static_cast<float>(n.lvl) / 1000.0f;
		drawNeedProgress(needNames[nIdx], needLvl);
		++nIdx;
	}

	ImGui::PopStyleColor(4);
}

inline void drawStatProgress(const std::string& sTitle, float lvlFrac, const int lvl)
{
	static const ImVec4 greenBar = ImColor(IM_COL32(0, 154, 0, 255)); 
	ImVec4 col = greenBar;
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_PlotHistogram, col);

	ImGui::Text(sTitle.c_str()); ImGui::SameLine();
	ImGui::ProgressBar(lvlFrac); ImGui::SameLine();
	ImGui::Text(std::to_string(lvl).c_str());

	ImGui::PopStyleColor();
}

void DwarfInfoGui::drawStats(const Entity & e) // TODO: Add in coloring based on how high level the entites skill is!
{
	auto& stats = e.getComponent<Stats>();

	ImGui::Text("Attributes"); ImGui::SameLine();

	// Find the longest character string so we can make things aesthetically pleasing
	int largest = 0;
	for (const auto& a : stats.attributes)
		if (a.first.size() > largest)
			largest = a.first.size();

	ImGui::Text("Progress Toward Lvl    "); ImGui::SameLine();

	ImGui::Text("LVL");

	for (const auto& a : stats.attributes)
	{
		auto attrProgress = static_cast<float>(a.second.experience) / static_cast<float>(attrLvlXpCap(a.second.attributeLvl));
		
		auto attrName = a.first;
		attrName.append(largest - a.first.size(), ' '); // TODO: Align all skills and attributes with the longest of both?

		drawStatProgress(attrName, attrProgress, a.second.attributeLvl);
	}

	ImGui::Text("Skills");

	// Same thing with skills as above
	// Except we have to grab the name instead of the tag
	largest = 0;
	for (const auto& s : stats.skills)
	{
		const auto* skillName = getSkillName(s.first);
		if (skillName->size() > largest)
			largest = skillName->size();
	}

	for (const auto& s : stats.skills)
	{
		const auto* skillName = getSkillName(s.first);
		auto skillProgress = static_cast<float>(s.second.experience) / static_cast<float>(skillLvlXpCap(s.second.skillLvl));

		auto fixedName = *skillName;
		fixedName.append(largest - s.first.size(), ' ');

		drawStatProgress(fixedName, skillProgress, s.second.skillLvl);
	}
}

void DwarfInfoGui::drawInventory(const Entity & e)
{

}
