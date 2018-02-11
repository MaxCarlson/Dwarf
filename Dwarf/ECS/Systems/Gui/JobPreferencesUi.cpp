#include "stdafx.h"
#include "JobPreferencesUi.h"
#include "Raws\SkillReader.h"
#include "ECS\Components\Sentients\Stats.h"
#include "Globals\game_states.h"
#include <imgui.h>
#include <imgui_tabs.h>

static const auto& skills = allSkills();

void JobPreferencesUi::init()
{
}

float findLongestSkillName()
{
	float szx = 0.0;
	for (const auto& sk : skills)
	{
		auto size = ImGui::CalcTextSize(sk.c_str()).x;
		if (size > szx)
		{
			szx = size;
		}
	}
	return szx;
}

void drawJobPrefrences(const Entity &e)
{
	ImGui::Begin("Skills", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	auto& skillPrefs = e.getComponent<AiWorkComponent>().jobPrefrences;

	static const float longestText = findLongestSkillName();

	int i = 0;
	for (const auto& sk : skills) 
	{
		const auto sizeX = ImGui::CalcTextSize(sk.c_str()).x;

		ImGui::AlignTextToFramePadding(); 
		ImGui::Text(getSkillName(sk)->c_str()); 
		ImGui::SameLine();
		ImGui::SetCursorPosX(longestText);

		auto find = skillPrefs.find(sk);

		if (find == skillPrefs.end())
			skillPrefs[sk] = 10;
		
		std::string label = "##JobPreferencesSkillsx" + std::to_string(i); 

		ImGui::SliderInt(label.c_str(), &skillPrefs[sk], 0, 20);
		++i;
		
		//ImGui::NewLine();
	}

	ImGui::End();
}

void JobPreferencesUi::update(const double duration) // These preferece changes have no effect yet!
{
	ImGui::Begin("Job Preferences", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
	
	if (ImGui::Button("Close##JobsUI"))
	{
		gameState = GameState::PLAYING;
	}

	ImGui::Text("Set Skill-Based job preferences");
	ImGui::Text("If you set a preference to zero \ncreature will not perform jobs of that skill type");

	const auto& ents = getEntities();

	// Build list of names
	int i = 1;
	std::vector<std::string> names;
	for (const auto& d : ents)
	{
		names.emplace_back(std::string("Dwarf") + std::to_string(i));
		++i;
	}

	
	static int selected = 0; 
	ImGui::ListBox("Dwarves", &selected, names);

	drawJobPrefrences(ents[selected]);

	ImGui::End();
}
