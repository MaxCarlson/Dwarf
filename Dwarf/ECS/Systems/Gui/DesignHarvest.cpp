#include "stdafx.h"
#include "DesignHarvest.h"
#include "Raws\ReadPlants.h"
#include "Raws\Defs\PlantDef.h"
#include "Globals\game_states.h"
#include "Globals\GlobalTerminals.h"
#include "Globals\TexDefines.h"
#include "mouse.h"
#include "KeyDampener.h"
#include "Map\Tile.h"
#include "Designations.h"
#include <imgui.h>
#include <imgui_tabs.h>
#include <DwarfRender.h>

void DesignHarvest::init()
{
}

void DesignHarvest::update(const double duration)
{
	ImGui::Begin("Farming##Design", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	if (ImGui::Button("Close##Harvest"))
	{
		gameState = GameState::PLAYING;
	}

	ImGui::Text("Right click to stop designating");

	if (mouse::rightClick)
		click = EMPTY_COORDINATES;

	// Set us to either drawing the highlighted area or confirming the command
	if (mouse::leftClick && !ImGui::IsMouseHoveringWindow())
	{
		if (click == EMPTY_COORDINATES)
			click = mouse::mousePos;
		else
			confirm = true;
	}

	ImGui::BeginTabBar("##FarmingTabs");

	if (ImGui::AddTab("Harvest##HarvestDesign"))
	{
		designHarvest();
	}

	if (ImGui::AddTab("Farming##HarvestDesign"))
	{
		designFarming();
	}

	ImGui::EndTabBar();
	ImGui::End();
}

void loopThroughHarvest(int type, Coordinates sml, Coordinates lrg, std::function<void(int, int, bool, std::string)> func)
{
	adjustCoordinatesForLoop(sml, lrg);

	overlayTerm->setAlpha(165);
	for (int x = sml.x; x <= lrg.x; ++x)
		for (int y = sml.y; y <= lrg.y; ++y)
		{
			const auto idx = getIdx({ x, y, sml.z });
			const auto plantLfc = region::plantLifeCycle(idx);
			const auto plantIdx = region::plantType(idx);

			bool passed = false;
			std::string product = "none";
			if (plantIdx > 0)
			{
				// And plant products to map
				const auto* plant = getPlantDef(plantIdx);
				product = plant->harvestsTo[plantLfc];

				// Filter out non productive harvests if we're not
				// going to be harvesting them
				if (type == 1 || product != "none")
				{
					passed = true;
				}
			}
			
			func(x, y, passed, product);
		}
}

void DesignHarvest::designHarvest()
{
	using namespace mouse;

	designations->harvest;

	ImGui::Text("Click and draw to designate \n an area for harvest.");
	ImGui::Text("Farmed plots will be auto harvested once plants have");
	ImGui::Text("reached maturity");

	static int selectedType = 0;
	ImGui::RadioButton("Harvest Productive", &selectedType, 0); // Tabs are glitching with radio buttons! FIX
	ImGui::RadioButton("Harvest All", &selectedType, 1); 


	// Draw harvest area
	int total = 0;
	std::unordered_map<std::string, int> products;
	
	if (click != EMPTY_COORDINATES)
	{
		loopThroughHarvest(selectedType, mousePos, click, [&products, &total](int x, int y, bool passed, std::string produce)
		{
			if (passed)
			{
				auto find = products.find(produce);

				if (find == products.end())
					products[produce] = 1;

				else
					++find->second;

				overlayTerm->setChar(x, y, { SQUARE_X_TEX,{ 0, 235, 0 }, { 0, 235, 0 } });
				
			}
			else
				overlayTerm->setChar(x, y, { SQUARE_X_TEX,{ 35, 0, 0 }, { 35, 0, 0 } });
		});
	}

	std::stringstream ss;
	ss << "Harvest Products \n";
	ss << "Total #: " << total << "\n";
	for (const auto& p : products)
		ss << p.first << " x " << p.second << "\n";

	ImGui::Text(ss.str().c_str());

	if (confirm)
	{
		const int z = click.z;
		loopThroughHarvest(selectedType, mousePos, click, [&products, &total, &z](int x, int y, bool passed, std::string produce)
		{
			if (passed)
			{
				designations->harvest.emplace_back(std::make_pair(false, Coordinates{ x, y, z }));
			}
		});

		confirm = false;
		click = EMPTY_COORDINATES;
	}
}

void DesignHarvest::designFarming()
{
	using namespace mouse;




	// Draw farm area
	if (click != EMPTY_COORDINATES)
	{
		Coordinates sml = click;
		Coordinates lrg = mousePos;

		adjustCoordinatesForLoop(sml, lrg);

		for (int x = sml.x; x <= lrg.x; ++x)
			for (int y = sml.y; y <= lrg.y; ++y)
			{

			}
	}



	if (confirm)
	{

		confirm = false;
		click = EMPTY_COORDINATES;
	}
}
