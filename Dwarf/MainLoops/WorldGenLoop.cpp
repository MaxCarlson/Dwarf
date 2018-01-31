#include "stdafx.h"
#include "WorldGenLoop.h"
#include "WorldGeneration.h"
#include "Map\building\PlanetBuilding.h"
#include "Raws\BiomeReader.h"
#include "Raws\Defs\BiomeDef.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <DwarfRender.h>

namespace Details
{
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
		CHOOSE_EMBARK
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

		ImGui::SliderInt("Planet X", &planetX, 32, 256);
		ImGui::SliderInt("Planet Y", &planetY, 32, 256);
		ImGui::SliderInt("Water Level", &waterD, 1, 4);
		ImGui::SliderInt("Plains Level", &plainsD, 1, 4);

		ImGui::Text("World Seed");
		ImGui::SameLine();
		ImGui::InputText("seed", (char *)&seed, 254);

		if (ImGui::Button("Ready"))
		{
			generateWorld(seed, planetX, planetY, { 80, 80, 150 }, 3, 3, 7);
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

	else if (stage == Stage::CHOOSE_EMBARK)
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

			//ImGui::IsMouseClicked(int{});
			
		}
	}
}

