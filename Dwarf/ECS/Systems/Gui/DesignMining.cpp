#include "stdafx.h"
#include "DesignMining.h"
#include "Globals\game_states.h"
#include "Globals\TexDefines.h"
#include "mouse.h"
#include "KeyDampener.h"
#include "Raws\Materials.h"
#include "Raws\Defs\MaterialDef.h"
#include "Raws\ItemRead.h"
#include "Raws\Defs\ItemDefs.h"
#include "Map\Tile.h"
#include "Designations.h"
#include "ECS\Messages\designation_message.h"
#include <imgui.h>
#include <DwarfRender.h>

static const std::vector<std::string> miningAdjectives = { "Mined", "Channeled", "Ramped", "Up Staired", "Down Staired", "Up/Down Staired", "Erased" };

void DesignMining::init()
{
}

void DesignMining::update(const double duration) // Add in mining templates!
{
	ImGui::Begin("Mining##Design", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	static std::string close = " Close##Mining";

	if (ImGui::Button(close.c_str()))
	{
		gameState = GameState::PLAYING;
	}

	static const char * miningModesStr = "Mine (d)\0Channel (c)\0Ramp (r)\0Up Stair (u)\0Down Stair (j)\0Up/Down Stair (x)\0Erase Designation (e)\0\0";

	ImGui::Text("Right click to cancel designation");
	ImGui::Text("Mining Modes: ");
	ImGui::SameLine();
	ImGui::Combo("##MiningModes", &miningType, miningModesStr);

	if (mouse::rightClick)
	{
		click = EMPTY_COORDINATES;
	}

	if (mouse::leftClick || click != EMPTY_COORDINATES)
	{
		if (mouse::leftClick && click != EMPTY_COORDINATES)
			confirm = true;

		if(click == EMPTY_COORDINATES)
			click = mouse::mousePos;

		drawPossibleMining();
	}

	ImGui::End();
}

inline bool outOfBounds(Coordinates co)
{
	if (co.x < 1 || co.x > MAP_WIDTH - 2 || co.y < 1 || co.y > MAP_HEIGHT - 2 || co.z < 2 || co.z > MAP_DEPTH - 2)
		return true;

	return false;
}

void DesignMining::loopThroughPossibleMining(int type, Coordinates sml, Coordinates lrg, std::function<void(bool, int, int)> onPossible)
{
	adjustCoordinatesForLoop(sml, lrg);

	for(int x = sml.x; x <= lrg.x; ++x)
		for (int y = sml.y; y <= lrg.y; ++y)
		{
			auto idx = getIdx({ x, y, sml.z });
			bool possible = true;

			auto find = designations->mining.find(idx);

			if (find != designations->mining.end() || outOfBounds({ x, y, sml.z }))
				possible = false;

			else
			{
				using region::TileTypes;
				auto tileType = region::getTileType(idx);

				switch (type)
				{
				case MINING:
				case RAMPING:
				case UP_STAIRS:
				case UP_DOWN_STAIRS:
					if (tileType != TileTypes::SOLID)
						possible = false;

					break;

				case CHANNELING:
				case DOWN_STAIRS:
					if (tileType != TileTypes::FLOOR)
						possible = false;

					break;
				}
			}

			onPossible(possible, x, y);
		}
}

// Build a map of all yields that will be produced
// if a sector is mined
void calculateMinedOre(const Coordinates co, std::unordered_map<std::string, int> &products)
{
	const int matIdx = region::getTileMaterial(co);

	auto* mat = getMaterial(matIdx);

	const auto& itag = mat->minesToTag;

	auto* item = getItemDef(itag);

	std::string itemStr = mat->name + " " + item->name;

	auto find = products.find(itemStr);

	if (find != products.end())
		find->second += mat->minesToAmount;
	else
	{
		products[itemStr] = 1;
	}
}

void DesignMining::drawPossibleMining()
{
	using namespace mouse;

	vchars ch;
	switch (miningType)
	{
	case MINING:
		ch = { 219,{ 255, 255, 0 },{} };
		break;
	case CHANNELING:
		ch = { 25, {255, 255, 0}, {255, 255, 255} };
		break;

	case RAMPING:
		ch = { RAMP_UP,{ 155, 155, 155 },{} };
		break;

	case UP_STAIRS:
		ch = { STAIRS_UP,{ 155, 155, 155 },{} };
		break;

	case DOWN_STAIRS:
		ch = { STAIRS_DOWN,{ 155, 155, 155 },{} };
		break;

	case UP_DOWN_STAIRS:
		ch = { STAIRS_UD,{ 155, 155, 155 },{} };
		break;
	}

	Coordinates sml = click;
	Coordinates lrg = mousePos;

	static auto* lterm = dfr::term(1);

	std::unordered_map<std::string, int> products;
	
	int totalPossible = 0;
	loopThroughPossibleMining(miningType, click, mousePos, [&totalPossible, &products, &ch, &sml](bool possible, int x, int y)
	{
		if (possible)
		{
			calculateMinedOre({ x, y, sml.z }, products);
			lterm->setChar(x, y, ch);
			++totalPossible;
		}
		else
		{

		}
	});

	std::stringstream ss;
	ss << "Coordinates: " << sml.x << "," << sml.y << "," << sml.z << " : " << lrg.x << "," << lrg.y << "," << lrg.z << "\n";
	ss << "Total Tiles to be " << miningAdjectives[miningType] << ": " << totalPossible;
	ImGui::Text(ss.str().c_str());
}

void DesignMining::designate()
{
}

void DesignMining::drawErasure()
{
}
