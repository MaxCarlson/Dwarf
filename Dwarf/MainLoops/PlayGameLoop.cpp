#include "stdafx.h"
#include "PlayGameLoop.h"
#include "WorldGenLoop.h"
#include "Globals\game_states.h"
#include "Globals\GlobalWorld.h"
#include "RunSystems.h"
#include "Map\building\PlanetBuilding.h"
#include "Map\Tile.h"
#include "Globals\global_calender.h"
#include "Globals\MainFunction.h"
#include "RegisterComponents.h"
#include "MainMenuLoop.h"
#include "Designations\Designations.h"
#include "Raws\DefInfo.h"
#include "Globals\Camera.h"
//#include "Map\building\regionBuilder.h"
//#include "Helpers\Rng.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include "KeyDampener.h"
#include <SFML\Window.hpp>
#include <DwarfRender.h>


#include <filesystem>
namespace fs = std::experimental::filesystem; 

static const std::string savePath = "Saves";

void saveGame(bool& done)
{
	static std::string path = "";
	ImGui::Begin("Save Game", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::Button("Back"))
	{
		done = true;
	}

	ImGui::Text("Enter the name of the save or click a save to overwrite");
	ImGui::InputText("File name", (char *)path.c_str(), 250);

	std::vector<std::string> paths; // We want these to update as they're added
	for (const auto &p : fs::directory_iterator(savePath))
		paths.emplace_back(p.path().string());

	static int selected = 0;
	ImGui::ListBox("", &selected, paths);

	if (ImGui::Button("Save") && !done)
	{
		path = "Saves/" + path;
		if (path == "Saves/")
			path = paths[selected];


		std::ofstream os(path, std::ios::binary);
		cereal::BinaryOutputArchive archive(os);

		// Save Planet then region then ECS world
		archive(planet);
		region::save_region(path);
		world.save(archive);

		// Misc archives, move somewhere else
		archive(calender);
		archive(designations);
		archive(defInfo);
		archive(camera);

		done = true;
		path = "";
	}

	ImGui::End();
}

// Clean up systems, entities, components,
// world, and variadic systems
inline void cleanUp()
{
	constexpr bool reset = true;
	ComponentsInit::registerOrResetVardiadicEach(reset);
	RunSystems::cleanSystems();
}

void loadGame(bool& done, bool &back)
{
	ImGui::SetNextWindowPosCenter();
	ImGui::Begin("Load Game", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::Button("Back"))
	{
		// This needs to be implemented differntly for main menu vs. in game play
		back = true;
	}

	std::vector<std::string> paths; // We want these to update as they're added
	for (const auto &p : fs::directory_iterator(savePath))
		paths.emplace_back(p.path().string());

	static int selected = 0;
	ImGui::ListBox("", &selected, paths);

	if (ImGui::Button("Load"))
	{
		cleanUp();

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
		RunSystems::initSystems(false);
		ComponentsInit::registerOrResetVardiadicEach(false);

		gameState = GameState::PLAYING;
	}
	
	else if (gameState == GameState::LOAD_GAME)
	{
		bool back = false;
		static bool loaded = false;

		loadGame(loaded, back);

		if (loaded)
		{
			loaded = false;
			gameState = GameState::PLAYING;
			RunSystems::initSystems(true);
			ComponentsInit::registerOrResetVardiadicEach(false);
		}
		if (back)
		{
			gameState = GameState::PLAYING;
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

	else if (gameState == GameState::QUIT_GAME)
	{
		ImGui::Begin("Quit to Main Menu", nullptr, ImVec2{ 600, 400 }, 0.7f, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);

		if (ImGui::Button("Back##QuitToMainMenu"))
		{
			gameState = GameState::PLAYING;
		}

		// TODO: Save and Quit to Main
		// TODO: Quit to Desktop?

		if (ImGui::Button("Quit##QuitToMainMenu"))
		{
			cleanUp();
			MainFunction = MainMenuLoop::run;
		}

		ImGui::End();
	}

	if (gameState == GameState::PLAYING || gameState == GameState::DESIGN || gameState == GameState::PAUSED)
	{
		RunSystems::updateSystems(duration);

		if (keys::isKeyDown(sf::Keyboard::F12, true))
		{
			static int i = 0; // Add a gui for this, and make it auto not overwrite existing files
			static const std::string ss = "ScreenShot ";
			dfr::takeScreenShot(ss + std::to_string(++i));
		}
	}

	if (keys::isKeyDown(sf::Keyboard::Space, true))
	{
		gameState = (gameState == GameState::PAUSED) ? GameState::PLAYING : GameState::PAUSED;
	}
}


