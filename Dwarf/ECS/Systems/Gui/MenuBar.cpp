#include "stdafx.h"
#include "MenuBar.h"
#include "Globals\game_states.h"
#include "Globals\global_calender.h"
#include <imgui.h>
#include <imgui-SFML.h>

const std::string mainMenu = "Main Menu";
const std::string design = "Design";
const std::string jobs = "Jobs";
const std::string dwarvesTitle = "Dwarves";

void MenuBar::init()
{
}

void MenuBar::update(const double duration)
{
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu(mainMenu.c_str()))
	{
		if (ImGui::MenuItem("Dev Mode"))
		{
			gameState = GameState::DESIGN;
			designState = DesignStates::DEV_MODE;
		}

		if (ImGui::MenuItem("Save Game"))
		{
			gameState = GameState::SAVE_GAME;
		}

		if (ImGui::MenuItem("Quite Game"))
		{
			gameState = GameState::QUIT_GAME;
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu(dwarvesTitle.c_str()))
	{
		if (ImGui::MenuItem("Info"))
		{
			designState = DesignStates::DWARF_INFO;
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu(design.c_str()))
	{
		if (ImGui::MenuItem("Build"))
		{
			gameState = GameState::DESIGN;
			designState = DesignStates::BUILD;
		}

		if (ImGui::MenuItem("Architecture"))
		{
			gameState = GameState::DESIGN;
			designState = DesignStates::ARCHITECURE;
		}

		if (ImGui::MenuItem("Mining"))
		{
			gameState = GameState::DESIGN;
			designState = DesignStates::MINING;
		}

		if (ImGui::MenuItem("Wood Cutting"))
		{
			gameState = GameState::DESIGN;
			designState = DesignStates::CHOP_TREES;
		}

		if (ImGui::MenuItem("Harvest"))
		{
			gameState = GameState::DESIGN;
			designState = DesignStates::HARVEST;
		}

		if (ImGui::MenuItem("Stockpiles"))
		{
			gameState = GameState::DESIGN;
			designState = DesignStates::STOCKPILES;
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu(jobs.c_str()))
	{
		if (ImGui::MenuItem("Job Preferences"))
		{
			gameState = GameState::DESIGN;
			designState = DesignStates::JOB_PREFERENCES;
		}

		if (ImGui::MenuItem("Work Orders"))
		{
			gameState = GameState::DESIGN;
			designState = DesignStates::WORK_ORDERS;
		}

		ImGui::EndMenu();
	}

	const std::string time = calender->getTime();

	ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::CalcTextSize(time.c_str()).x - 45);
	ImGui::Text(time.c_str());

	ImGui::EndMainMenuBar();
}
