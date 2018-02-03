#include "stdafx.h"
#include "PlayGameLoop.h"
#include "WorldGenLoop.h"
#include "Globals\game_states.h"
#include "Globals\GlobalWorld.h"
#include "RunSystems.h"
#include "RegisterComponents.h"
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

	std::string dirpath = "Saves";
	std::vector<std::string> paths;
	for (const auto &p : fs::directory_iterator(dirpath))
	{
		ImGui::Text(p.path().string().c_str());

		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			std::string dirpath = p.path().string();

			std::ifstream is(dirpath, std::ios::binary);
			cereal::BinaryInputArchive iarchive(is);

			// Load region then world
			// Systems aren't loaded, just re-created
			iarchive(planet);
			region::load_region(dirpath);
			world.load(iarchive);

			// Init misc maps and designations
			calender = std::make_unique<Calender>();
			defInfo = std::make_unique<DefInfo>();

			// Misc archives, move somewhere else
			iarchive(calender);
			iarchive(designations);
			iarchive(defInfo);
			iarchive(camera);

			done = true;
		}
	}

	ImGui::End();
}

void PlayGameLoop::run(const double duration)
{

	if (gameState == GameState::NEW_GAME)
	{
		//ImGui::SetNextWindowPosCenter();
		//ImGui::Begin("Pick Region Size", nullptr, ImVec2{ 600, 400 }, 0.7f, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);

		ComponentsInit::init(); // This isn't strictly neccasary if a new game has been started before, probably won't cause issues but keep a look out!
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
			ComponentsInit::init();
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

	if(gameState == GameState::PLAYING)
		RunSystems::updateSystems(duration);

	if (gameState == GameState::DESIGN) // Replace this eventually with a pause system
		RunSystems::updateSystems(0.0);
}


