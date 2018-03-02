#include "stdafx.h"
#include "MilitaryGui.h"
#include "Globals\GlobalWorld.h"
#include "Globals\game_states.h"
#include "ECS\Components\Sentients\AiWorkComponent.h"
#include "ECS\Components\Tags\MilitaryTag.h"
#include <imgui.h>


void MilitaryGui::update(const double duration)
{
	ImGui::Begin("Military Gui");

	if (ImGui::Button("Close##Military Gui"))
	{
		gameState = GameState::PLAYING;
	}

	ImGui::Text("Choose which dwarves you want in the military here");

	std::vector<Entity> allPawns;
	std::vector<Entity> activeMilitary;

	// Replace this with a Controlable settler tag or something (or rename)?
	std::unordered_set<size_t> notCivilians;
	eachWith<Requires<MilitaryTag>>([&](const Entity &e)
	{
		activeMilitary.emplace_back(e);
		notCivilians.emplace(e.getId().index);
	});

	eachWith<Requires<AiWorkComponent>>([&](const Entity &e)
	{
		// Don't list dwarves twice
		// we sepperate civ's from military personel
		if(notCivilians.find(e.getId().index) == notCivilians.end())
			allPawns.emplace_back(e);
	});

	ImGui::Columns(3); // TODO: Figure out how to visually reprsent dwarves, their stats, etc without it looking horrible!

	ImGui::End();
}
