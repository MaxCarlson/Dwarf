#include "stdafx.h"
#include "WorldGenLoop.h"
#include "PlayGameLoop.h"
#include "MainMenuLoop.h"
#include "Globals\MainFunction.h"
#include "Globals\game_states.h"
#include "WorldGeneration.h"
#include "Map\building\PlanetBuilding.h"
#include "Map\World\Planet.h"
#include "Map\building\regionBuilder.h"
#include "Helpers\Rng.h"
#include "Raws\BiomeReader.h"
#include "Raws\Defs\BiomeDef.h"
#include "EntityFactory.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <DwarfRender.h>

namespace Details
{
	int regionX = 64;
	int regionY = 64;
	int regionZ = 150;
	int numDwarves = 7;
	int planetX = 64;
	int planetY = 64;
	int waterD = 3;
	int plainsD = 3;
	std::string seed = "seed";

	int wxOffset = 5;
	int wyOffset = 2;

	enum Stage
	{
		NOT_GENERATING,
		GENERATING,
		CHOOSE_EMBARK,
		FINALIZE_EMBARK
	};

	Stage stage = NOT_GENERATING;

	bool showMouse = false;
	std::pair<int, int> mousePos;
}

using namespace Details;

void renderWorldGen()
{
	int x = wxOffset;
	int y = wyOffset;

	dfr::terminal->clear();

	for (const auto& v : worldGenDisplay)
	{
		if (x > dfr::terminal->width - 1)
		{
			++x;
			if (x > WORLD_WIDTH - 1 + wxOffset)
			{
				x = wxOffset;
				++y;
			}
			continue;
		}

		dfr::terminal->setChar(dfr::terminal->at(x, y), { static_cast<uint32_t>(v.c), v.fg, v.bg });

		if (showMouse)
		{
			terminal_color("grey");
			terminal_put(x, y, 0xE100 + 88);

			dfr::terminal->setChar(dfr::terminal->at(mousePos.first, mousePos.second), { static_cast<uint32_t>(88), {255, 205, 0}, { 0, 0, 0 } });
		}

		++x;
		if (x > WORLD_WIDTH - 1 + wxOffset)
		{
			x = wxOffset;
			++y;
		}
	}
}

void WorldGenLoop::run(const double duration)
{
	mousePos = dfr::getMousePosition();

	if (stage == Stage::NOT_GENERATING)
	{
		ImGui::SetNextWindowPosCenter();
		ImGui::Begin("Planet Building", nullptr, ImVec2{ 600, 400 }, 0.5f, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
		ImGui::Text("Enter the desired Planet dimensions then click 'Ready'");

		ImGui::SliderInt("Number of Dwarves", &numDwarves, 1, 10);
		ImGui::SliderInt("Region Width", &regionX, 32, 256);
		ImGui::SliderInt("Region Height", &regionY, 32, 256);
		ImGui::SliderInt("Region Depth", &regionZ, 32, 256);
		ImGui::SliderInt("Planet Width", &planetX, 32, 256);
		ImGui::SliderInt("Planet Height", &planetY, 32, 256);
		ImGui::SliderInt("Water Level", &waterD, 1, 4);
		ImGui::SliderInt("Plains Level", &plainsD, 1, 4);

		ImGui::Text("World Seed");
		ImGui::SameLine();
		ImGui::InputText("seed", (char *)&seed, 254);

		if (ImGui::Button("Ready"))
		{
			generateWorld(seed, planetX, planetY, { regionX, regionY, regionZ }, waterD, plainsD, numDwarves);
			stage = Stage::GENERATING;
		}


		ImGui::End();
	}
	else 
	{
		renderWorldGen();
	}

	if (worldGenDone && stage == Stage::GENERATING)
	{
		ImGui::Begin("Planet Acceptable", nullptr, ImVec2{ 600, 400 }, 0.7f, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
		ImGui::Text("Is World Acceptable?");
		
		if (ImGui::Button("Yes"))
		{
			stage = Stage::CHOOSE_EMBARK;
			showMouse = true;
		}

		if (ImGui::Button("No"))
		{
			stage = Stage::NOT_GENERATING;
			worldGenDone = false;
			worldGenDisplay.clear();
		}

		ImGui::End();
	}

	else if (stage == Stage::CHOOSE_EMBARK || stage == Stage::FINALIZE_EMBARK)
	{
		if (mousePos.first >= 0 + wxOffset && mousePos.first < WORLD_WIDTH + wxOffset
			&& mousePos.second >= 0 + wyOffset && mousePos.second < WORLD_HEIGHT + wyOffset)
		{
			const auto& tile = planet.tiles[planet.idx(mousePos.first - wxOffset, mousePos.second - wyOffset)];

			std::string biomeName = "No Biome!";

			if (tile.biomeIdx > -1)
			{
				auto bidx = planet.biomes[tile.biomeIdx].type;
				biomeName = getBiomeDef(bidx)->name;
			}

			ImGui::Begin("Tile Info", nullptr, ImVec2{ 600, 400 }, 0.7f, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);

			ImGui::Text(std::string("Tile Biome " + biomeName).c_str());
			ImGui::Text(std::string("Tile Height " + std::to_string(tile.height)).c_str());
			ImGui::Text(std::string("Tile Rainfall " + std::to_string(tile.rainfall)).c_str());
			ImGui::Text(std::string("Tile Temperature " + std::to_string(tile.temperature)).c_str());
			ImGui::Text(std::string("Tile Variance " + std::to_string(tile.variance)).c_str());

			ImGui::End();
		}

		if (ImGui::IsMouseClicked(dfr::Button::LEFT) || stage == Stage::FINALIZE_EMBARK)
		{
			stage = Stage::FINALIZE_EMBARK;

			ImGui::SetNextWindowPosCenter();
			ImGui::Begin("Finalize Embark", nullptr, ImVec2{ 600, 400 }, 0.7f, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
			ImGui::Text("Really Embark Here?");

			if (ImGui::Button("Yes"))
			{
				// Transfer control to the Play game loop and init systems and components
				MainFunction = PlayGameLoop::run;
				gameState = GameState::NEW_GAME;

				// Build the region
				buildRegion(planet, mousePos.first - wxOffset, mousePos.second - wyOffset, { regionX, regionY, regionZ }, rng);

				for (int i = 0; i < numDwarves; ++i) // Place this somewhere else once we have code to create dwarves with new gui
					createDwarf({});
			}

			if (ImGui::Button("No"))
			{
				stage = Stage::CHOOSE_EMBARK;
			}

			ImGui::End();
		}
	}
}

