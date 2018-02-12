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
#include "ECS\Systems\helpers\SeedsHelper.h"
#include "ECS\Components\Seed.h"
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

void loopThroughFarming(Coordinates sml, Coordinates lrg, std::function<void(int, int, bool)> func)
{
	adjustCoordinatesForLoop(sml, lrg);

	for (int x = sml.x; x <= lrg.x; ++x)
		for (int y = sml.y; y <= lrg.y; ++y)
		{
			bool possible = true;
			const int idx = getIdx({ x, y, sml.z });

			auto farmFind = designations->farming.find(idx);

			if (farmFind != designations->farming.end())
				possible = false;

			if (region::solid(idx)
				|| region::flag({ x, y, sml.z }, region::CONSTRUCTION)
				|| region::getTileType(idx) != region::TileTypes::FLOOR)
				possible = false;

			func(x, y, possible);
		}
}

void DesignHarvest::designFarming()
{
	using namespace mouse;

	ImGui::Text("Designate a farm plot of an availble seed type");

	enum { DRAW, ERASE };

	static int radioEraseOrDraw = 0;
	ImGui::RadioButton("Draw", &radioEraseOrDraw, DRAW);
	ImGui::RadioButton("Erase", &radioEraseOrDraw, ERASE);

	// Build a container of plant names that we have seeds for,
	// as well as plant tags and how many seeds for that tag
	std::map<std::string, std::pair<std::string, int>> availibleSeeds;
	seedsHelper.forAllUnclaimedSeeds([&availibleSeeds](Entity e)
	{
		auto& seed = e.getComponent<Seed>();
		
		auto plant = getPlantDef(getPlantIdx(seed.plantTag));

		if (plant)
		{
			auto find = availibleSeeds.find(seed.plantTag);

			if (find == availibleSeeds.end())
				availibleSeeds[plant->name] = std::make_pair(seed.plantTag, 1);
			else
				++find->second.second;
		}
	});

	// Add seed names to vector so we can make it filterable
	static ImGuiTextFilter seedFilter;
	seedFilter.Draw();

	ImGui::Text("Qty:");

	// Seed names that passed the filter
	// and the map index of those that did, with identical vector idx's
	std::vector<std::string> seedTags;
	std::vector<std::string> seedNames;
	for (const auto& s : availibleSeeds)
	{
		if (seedFilter.PassFilter(s.first.c_str()))
		{
			seedNames.emplace_back(std::to_string(s.second.second) + " " + s.first);
			seedTags.emplace_back(s.first);
		}
	}

	if (seedNames.empty())
	{
		ImGui::Text("No Seeds :("); // NEED TO REFACTOR, selectedSeedQty below does not allow us to enter deleting farm plots  :(
		return;
	}

	static int selected = 0;
	ImGui::ListBox("Availible Seeds", &selected, seedNames);

	const int selectedSeedQty = availibleSeeds[seedTags[selected]].second;

	// Draw farm area
	int totalArea = 0;
	if (click != EMPTY_COORDINATES && availibleSeeds.size() > 0)
	{
		if (radioEraseOrDraw == DRAW)
		{
			loopThroughFarming(mouse::mousePos, click, [&totalArea, &selectedSeedQty](int x, int y, bool possible)
			{
				if (possible)
				{
					overlayTerm->setChar(x, y, { SQUARE_X_TEX, { 0, 235, 0}, {} });
					++totalArea;
				}
				else
					overlayTerm->setChar(x, y, { SQUARE_X_TEX, { 235, 0, 0}, {} });
			});
		}
	}

	if (totalArea > selectedSeedQty)
		ImGui::Text("You don't have enough seeds!");

	// Still allow user to designate a farm plot of
	// a seed type if they don't have enough seeds. They only need 1
	if (confirm)
	{
		const int z = click.z;
		loopThroughFarming(mouse::mousePos, click, [&z, &seedTags](int x, int y, bool possible)
		{
			auto idx = getIdx({ x, y, z });
			auto farmFind = designations->farming.find(idx);

			if (farmFind == designations->farming.end())
			{
				size_t seedId = 0;
				bool found = false;
				seedsHelper.forAllUnclaimedSeeds([&found, &seedTags, &seedId](Entity e)
				{
					auto& tag = e.getComponent<Seed>().plantTag;
					if (!found && tag == seedTags[selected])
					{
						found = true;
						e.addComponent<Claimed>();
					}
				});

				if (found)
					designations->farming[idx] = FarmInfo{ FarmInfo::CLEAR, seedId, seedTags[selected] };
			}

			else if (radioEraseOrDraw == ERASE)
				designations->farming.erase(idx);

		});

		confirm = false;
		click = EMPTY_COORDINATES;
	}
}
