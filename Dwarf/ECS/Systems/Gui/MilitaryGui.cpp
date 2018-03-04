#include "stdafx.h"
#include "MilitaryGui.h"
#include "Globals\GlobalWorld.h"
#include "Globals\game_states.h"
#include "ECS\Components\Sentients\Name.h"
#include "ECS\Components\Sentients\AiWorkComponent.h"
#include "ECS\Components\Tags\MilitaryTag.h"
#include <imgui.h>

static std::array<char, 30> squadInFocusName = { ' ' };

void editSquad(const std::string &name)
{

}

template<bool remove>
inline void addOrRemoveFromSquad(std::string squadName, const std::vector<Entity>& entsToSearch)
{
	if (remove)
		ImGui::Text("Select to remove from squad");
	else
		ImGui::Text("Select to add to squad");

	bool confirm = false;
	if (remove && ImGui::Button("Remove##FromSquad"))
		confirm = true;
	else if (!remove && ImGui::Button("Add To Squad##Add"))
		confirm = true;


	int i = 0;
	static std::vector<bool> selected;
	selected.resize(entsToSearch.size(), false);

	for (const auto& e : entsToSearch)
	{
		std::stringstream ss;

		ss << i << " - " << getFullName(e);

		if (ImGui::Selectable(ss.str().c_str(), selected[i]))
			selected[i] = !selected[i];

		++i;
	}

	if (confirm)
	{
		i = 0;
		for (auto e : entsToSearch)
		{
			if (selected[i] && remove)
			{
				e.removeComponent<MilitaryTag>();
				e.activate();
			}
			if (selected[i] && !remove)
			{
				e.addComponent<MilitaryTag>(squadName);
				e.activate();
			}
			++i;
		}
		std::fill(selected.begin(), selected.end(), false);
	}

}


bool createSquad(const std::vector<Entity>& allPawns, const std::unordered_map<std::string, std::vector<Entity>>& activeMilitary) // ISSUE: This will fail if entities have the same name
{																																  // TODO: Merge Edit squad into this func?
	ImGui::Begin("Create Squad##Military");
	bool done = false;

	if (ImGui::Button("Back"))
		done = true;

	// TODO: Allow for filtering / ordering by different skills
	// TODO: Show combat skill levels
	// TODO: Show all of a creatures info when selected

	ImGui::InputText("Squad Name: ", &squadInFocusName[0], 25);

	// TODO: Add name Filter
	// TODO: Add skill level filter in various skills

	ImGui::Columns(2);
	ImGui::Text("Squad Candidates");

	addOrRemoveFromSquad<false>(squadInFocusName.data(), allPawns);

	ImGui::NextColumn();

	auto findSquad = activeMilitary.find(squadInFocusName.data());

	if(findSquad != activeMilitary.end())
		addOrRemoveFromSquad<true>(squadInFocusName.data(), findSquad->second);

	ImGui::End();
	return !done;
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
			creatingSquad = true;
			std::fill(squadInFocusName.begin(), squadInFocusName.end(), ' ');
			
			//squadName.copy(&squadInFocusName[0], squadName.size());
			int strIdx = 0;
			for (const auto& l : sq.first) { squadInFocusName[strIdx] = l; ++strIdx; }
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

		ImGui::Separator();
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
