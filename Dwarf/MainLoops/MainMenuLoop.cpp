#include "stdafx.h"
#include "MainMenuLoop.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "DwarfRender.h"
#include "Globals\MainFunction.h"
#include "WorldGenLoop.h"
#include "Globals\game_states.h"
#include "PlayGameLoop.h"
#include "RegisterComponents.h"

static const std::string loadGame = "Load Game";
static const std::string createWorld = "Create World";
static const std::string quitGame = "Quite Game";


float calcIndent(const float width, const float textWidth)
{
	return width / 2.0 - textWidth / 2.0;
}

static bool first = true;

void MainMenuLoop::run(const double duration)
{
	if (first)
	{
		first = false;
		// Register all used components so they stay in same order
		// on serialization / deserialization
		ComponentsInit::init();
	}
		

	ImGui::SetNextWindowPosCenter();
	ImGui::Begin("MainMenu", nullptr, ImVec2{ ImGui::CalcTextSize(createWorld.c_str()).x, 400 }, 0.0f,
		ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoTitleBar);


	//ImGui::TextColored(yellow, "%s", "(c)");

	if (ImGui::Button(loadGame.c_str()))
	{
		gameState = GameState::LOAD_GAME;
		MainFunction = PlayGameLoop::run;
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
