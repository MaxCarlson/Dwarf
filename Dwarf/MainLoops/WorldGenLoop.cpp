#include "stdafx.h"
#include "WorldGenLoop.h"
#include "WorldGeneration.h"
#include <imgui.h>
#include <imgui-SFML.h>

namespace Details
{
	int planetX = 64;
	int planetY = 64;
	std::string seed = "seed";
}


void WorldGenLoop::run(const double duration)
{
	using namespace Details;

	ImGui::SetNextWindowPosCenter();
	ImGui::Begin("Planet Building", nullptr, ImVec2{ 600, 400 }, 0.7f, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
	ImGui::Text("Enter the desired Planet dimensions then click 'Ready'");

	ImGui::Text("Planet X");
	ImGui::SameLine();
	ImGui::InputInt("## planet x", &planetX);

	ImGui::Text("Planet Y");
	ImGui::SameLine();
	ImGui::InputInt("## planet y", &planetY);

	ImGui::Text("World Seed");
	ImGui::SameLine();
	ImGui::InputText("seed", (char *)&seed, 254);

	if (ImGui::Button("Ready"))
	{
		generateWorld(seed, planetX, planetY, { 80, 80, 150 }, 3, 3, 7);
	}

	ImGui::End();
}

