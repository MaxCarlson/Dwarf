#include "stdafx.h"
#include "MilitaryGui.h"
#include "Globals\GlobalWorld.h"
#include "Globals\game_states.h"
#include "ECS\Components\Sentients\Name.h"
#include "ECS\Components\Sentients\AiWorkComponent.h"
#include "ECS\Components\Tags\MilitaryTag.h"
#include <imgui.h>

void editSquad()
{
	ImGui::Begin("Edit Squad##Military");

	ImGui::End();
}

bool createSquad(const std::vector<Entity>& allPawns, const std::unordered_map<std::string, std::vector<Entity>>& activeMilitary) // ISSUE: This will fail if entities have the same name
{																																  // TODO: Merge Edit squad into this func?
	ImGui::Begin("Create Squad##Military");

	// TODO: Allow for filtering / ordering by different skills
	// TODO: Show combat skill levels
	// TODO: Show all of a creatures info when selected

	std::array<char, 40> squadName;
	ImGui::InputText("Squad Name: ", &squadName[0], 40);

	static std::unordered_map<std::string, Entity> currentSquad;

	// TODO: Add name Filter
	// TODO: Add skill level filter in various skills

	ImGui::Columns(2);
	ImGui::NextColumn();

	ImGui::Text("Squad Candidates");

	for (const auto& e : allPawns)
	{
		if (!e.hasComponent<Name>()) continue;

		const auto& name = getFullName(e);

		// Don't show pawns already selected for squad as availible to add to squad
		if (currentSquad.find(name) != currentSquad.end())
			continue;

		ImGui::Text(name.c_str()); ImGui::SameLine();

		std::string addToSquadName = "Add##" + name;

		if (ImGui::Button(addToSquadName.c_str())) 
			currentSquad.emplace(name, e);
	}

	ImGui::NextColumn();

	int i = 0;
	ImGui::Text("In Squad");
	for (auto it = currentSquad.begin(); it != currentSquad.end(); ++it, ++i)
	{
		std::stringstream ss;

		ss << i << " - " << it->first;

		ImGui::Text(it->first.c_str()); ImGui::SameLine();

		std::string removeFromSquad = "Remove##" + it->first; // TODO: Make individually selectable to see all info about entity

		if (ImGui::Button(removeFromSquad.c_str()))
			it = currentSquad.erase(it);
		
	}

	bool notDone = true;
	if(ImGui::Button("Finalize##CreateSquad") && !squadName.empty())
	{
		for (auto& ep : currentSquad)
			ep.second.addComponent<MilitaryTag>(MilitaryTag{ squadName.data() });

		notDone = false;
		currentSquad.clear();
	}

	ImGui::End();
	return notDone;
}

void displaySquads(const std::vector<Entity>& allPawns, const std::unordered_map<std::string, std::vector<Entity>>& activeMilitary)
{
	static bool creatingSquad = false;
	if (ImGui::Button("Create New Squad##Military") || creatingSquad)
	{
		creatingSquad = createSquad(allPawns, activeMilitary);
	}

	for (const auto& sq : activeMilitary)
	{
		std::string squadName = "Squad: " + sq.first;
		ImGui::Text(squadName.c_str()); ImGui::SameLine();

		std::string editButton = "Edit Squad##" + sq.first;
		if (ImGui::Button(editButton.c_str())) // TODO: Create vector of bools denoting which squad is being edited
		{
		}

		int i = 1;
		for (const auto& ent : sq.second)
		{
			std::stringstream ss;
			auto& name = ent.getComponent<Name>();

			ss << i << " - " << name.firstName << " " << name.lastName;

			// TODO: Add a button to allow the viewing of a squad members info from here
			// TODO: Show combat role ?
			// TODO: Show current Duty
			// TODO: Show squad schedule

			ImGui::Text(ss.str().c_str());
			++i;
		}

		ImGui::Indent();
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
	std::unordered_set<size_t> notCivilians;
	std::unordered_map<std::string, std::vector<Entity>> activeMilitary;

	eachWith<Requires<MilitaryTag>>([&](const Entity &e)
	{
		auto mil = e.getComponent<MilitaryTag>();

		auto sFind = activeMilitary.find(mil.squadTag);

		// Build the map of entities that are in the military
		// and index them by their squad tag
		if (sFind == activeMilitary.end())
			activeMilitary.emplace(mil.squadTag, std::vector<Entity>{ e });
		
		else
			sFind->second.emplace_back(e);

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
