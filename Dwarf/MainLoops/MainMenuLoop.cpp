#include "stdafx.h"
#include "MainMenuLoop.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "DwarfRender.h"
#include "Globals\MainFunction.h"
#include "WorldGenLoop.h"

static const std::string loadGame = "Load Game";
static const std::string createWorld = "Create World";
static const std::string quitGame = "Quite Game";


float calcIndent(const float width, const float textWidth)
{
	return width / 2.0 - textWidth / 2.0;
}

void MainMenuLoop::run(const double duration)
{
	ImVec4 yellow{ 1.0f, 1.0f, 0.0f, 1.0f };

	//const auto twidth = ImGui::GetWindowWidth();
	//const auto theight = ImGui::GetWindowHeight();

	ImGui::SetNextWindowPosCenter();
	ImGui::Begin("MainMenu", nullptr, ImVec2{ ImGui::CalcTextSize(createWorld.c_str()).x, 400 }, 0.0f,
		ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoTitleBar);


	//ImGui::TextColored(yellow, "%s", "(c)");

	if (ImGui::Button(loadGame.c_str()))
	{
		
	}

	if (ImGui::Button(createWorld.c_str()))
	{
		MainFunction = WorldGenLoop::run;
	}

	if (ImGui::Button(quitGame.c_str()))
	{

	}
	


	ImGui::End();
}
