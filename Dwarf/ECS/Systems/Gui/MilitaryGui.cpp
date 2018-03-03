#include "stdafx.h"
#include "MilitaryGui.h"
#include "Globals\GlobalWorld.h"
#include "Globals\game_states.h"
#include "ECS\Components\Sentients\AiWorkComponent.h"
#include "ECS\Components\Tags\MilitaryTag.h"
#include <imgui.h>

void createSquad(const std::vector<Entity>& allPawns, const std::vector<Entity>& activeMilitary)
{

}

void displaySquads(const std::vector<Entity>& allPawns, const std::unordered_map<std::string, Entity>& activeMilitary)
{
	static bool creatingSquad = false;
	if (ImGui::Button("Create New Squad##Military") || creatingSquad)
	{
		creatingSquad = true;
	}


}


void MilitaryGui::update(const double duration) // TODO: Need to show dwarf skills when one is selected in a new ImGui::Begin()
{
	ImGui::Begin("Military Gui");

	if (ImGui::Button("Close##Military Gui"))
	{
		gameState = GameState::PLAYING;
	}

	ImGui::Text("Choose which dwarves you want in the military here");

	std::vector<Entity> allPawns;
	std::unordered_map<std::string, Entity> activeMilitary;

	// Replace this with a Controlable settler tag or something (or rename)?
	std::unordered_set<size_t> notCivilians;
	eachWith<Requires<MilitaryTag>>([&](const Entity &e)
	{
		auto mil = e.getComponent<MilitaryTag>();

		activeMilitary.emplace(mil.squadTag, e);
		notCivilians.emplace(e.getId().index);
	});

	eachWith<Requires<AiWorkComponent>>([&](const Entity &e)
	{
		// Don't list dwarves twice
		// we sepperate civ's from military personel
		if(notCivilians.find(e.getId().index) == notCivilians.end())
			allPawns.emplace_back(e);
	});

	displaySquads(allPawns, activeMilitary);

	ImGui::End();
}
