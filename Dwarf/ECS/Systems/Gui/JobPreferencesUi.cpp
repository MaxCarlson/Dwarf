#include "stdafx.h"
#include "JobPreferencesUi.h"
#include "Raws\SkillReader.h"
#include <imgui.h>
#include <imgui_tabs.h>

void JobPreferencesUi::init()
{
}


void drawJobPrefrences(const Entity &e)
{
	static const auto& skills = allSkills();

	ImGui::Begin("Skills", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	auto& skillPrefs = e.getComponent<AiWorkComponent>().jobPrefrences;

	int i = 0;
	for (const auto& sk : skills)
	{
		ImGui::Text(sk.c_str());
		ImGui::SameLine();

		auto find = skillPrefs.find(sk);

		if (find == skillPrefs.end())
			skillPrefs[sk] = 0;
		
		std::string label = "##JobPreferences" + std::to_string(i);

		ImGui::SliderInt(label.c_str(), &skillPrefs[sk], 0, 20);
		++i;
	}

	ImGui::End();
}

void JobPreferencesUi::update(const double duration)
{
	ImGui::Begin("Job Preferences", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	const auto& ents = getEntities();

	// Build list of names
	int i = 0;
	std::vector<std::string> names;
	for (const auto& d : ents)
	{
		names.emplace_back(std::string("Dwarf" + std::to_string(i)));
	}

	
	static int selected = 0; 
	ImGui::ListBox("Dwarves", &selected, names);

	drawJobPrefrences(ents[selected]);

	ImGui::End();
}
