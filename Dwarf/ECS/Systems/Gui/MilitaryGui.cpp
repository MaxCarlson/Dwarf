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

	std::vector<Entity> allPawns;
	std::vector<Entity> activeMilitary;

	// Replace this with a Controlable settler tag or something (or rename)?
	eachWith<Requires<AiWorkComponent>>([&allPawns](const Entity &e)
	{
		allPawns.emplace_back(e);
	});

	eachWith<Requires<MilitaryTag>>([&activeMilitary](const Entity &e)
	{
		activeMilitary.emplace_back(e);
	});

	ImGui::End();
}
