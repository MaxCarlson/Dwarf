#include "stdafx.h"
#include "PlayGameLoop.h"
#include "WorldGenLoop.h"
#include "Globals\game_states.h"
#include "Globals\GlobalWorld.h"
#include "RunSystems.h"
#include "Map\building\PlanetBuilding.h"
#include "Map\Tile.h"
#include "Globals\global_calender.h"
#include "Designations.h"
#include "Raws\DefInfo.h"
#include "Globals\Camera.h"
//#include "Map\building\regionBuilder.h"
//#include "Helpers\Rng.h"
#include <imgui.h>
#include <imgui-SFML.h>

#include <filesystem>
namespace fs = std::experimental::filesystem; 


void saveGame(bool& done)
{
	static std::string path = "newGame";
	ImGui::Begin("Save Game", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::Button("Back"))
	{
		done = true;
		return;
	}

	ImGui::Text("Enter the name of the save");
	ImGui::InputText("File name", (char *)path.c_str(), 250);

	if (ImGui::Button("Save"))
	{
		std::string dirpath = "Saves/" + path;

		std::ofstream os(dirpath, std::ios::binary);
		cereal::BinaryOutputArchive archive(os);

		// Save Planet then region then ECS world
		archive(planet);
		region::save_region(dirpath);
		world.save(archive);

		// Misc archives, move somewhere else
		archive(calender);
		archive(designations);
		archive(defInfo);
		archive(camera);

		done = true;
	}

	ImGui::End();
}

void loadGame(bool& done)
{
	ImGui::SetNextWindowPosCenter();
	ImGui::Begin("Load Game", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::Button("Back"))
	{
		// This needs to be implemented differntly for main menu vs. in game play
		return;
	}

	std::string dirpath = "Saves";

	std::vector<std::string> paths; // We want these to update as they're added
	for (const auto &p : fs::directory_iterator(dirpath))
		paths.emplace_back(p.path().string());

	static int selected = 0;
	ImGui::ListBox("", &selected, paths);

	if (ImGui::Button("Load"))
	{
		std::string dirpath = paths.at(selected);

		std::ifstream is(dirpath, std::ios::binary);
		cereal::BinaryInputArchive iarchive(is);

		// Load region then world
		// Systems aren't loaded, just re-created
		iarchive(planet);
		region::load_region(dirpath);
		world.load(iarchive);

		// Init misc 
		calender = std::make_unique<Calender>();
		defInfo = std::make_unique<DefInfo>();

		// Misc archives, move somewhere else
		iarchive(calender);
		iarchive(designations);
		iarchive(defInfo);
		iarchive(camera);

		done = true;
	}

	ImGui::End();
}

void PlayGameLoop::run(const double duration)
{

	if (gameState == GameState::NEW_GAME)
	{
		//ImGui::SetNextWindowPosCenter();
		//ImGui::Begin("Pick Region Size", nullptr, ImVec2{ 600, 400 }, 0.7f, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);

		RunSystems::initSystems(false);

		gameState = GameState::PLAYING;
	}
	
	else if (gameState == GameState::LOAD_GAME)
	{
		static bool loaded = false;

		loadGame(loaded);

		if (loaded)
		{
			loaded = false;
			gameState = GameState::PLAYING;
			RunSystems::initSystems(true);
		}
	}

	else if (gameState == GameState::SAVE_GAME)
	{
		static bool saving = false;

		saveGame(saving);

		if (saving)
		{
			saving = false;
			gameState = GameState::PLAYING;
		}
	}

	if(gameState == GameState::PLAYING || gameState == GameState::DESIGN)
		RunSystems::updateSystems(duration);
}


