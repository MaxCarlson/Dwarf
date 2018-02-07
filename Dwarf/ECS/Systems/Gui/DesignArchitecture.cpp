#include "stdafx.h"
#include "DesignArchitecture.h"
#include "Globals\game_states.h"
#include "ECS\Messages\designate_architecture_message.h"
#include "mouse.h"
#include "Map\Tile.h"
#include "KeyDampener.h"
#include "Globals\TexDefines.h"
#include "Designations.h"
#include "Globals\GlobalTerminals.h"
#include <imgui.h>
#include <DwarfRender.h>


void DesignArchitecture::init()
{
}

void DesignArchitecture::update(const double duration)
{
	ImGui::Begin("Architecture##Design", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	static std::string close = " Close##DesignArch";
	
	if (ImGui::Button(close.c_str()))
	{
		gameState = GameState::PLAYING;
	}

	ImGui::Text("Right click when designating to stop designating");
	ImGui::Indent();
	ImGui::Text("Modes");
	ImGui::RadioButton("Draw", &mode, 0); ImGui::SameLine(); 
	ImGui::RadioButton("Erase all", &mode, 1); ImGui::SameLine();
	ImGui::RadioButton("Erase type", &mode, 2); 
	
	ImGui::RadioButton("Wall", &selected, ArchitectureType::WALL); ImGui::SameLine();
	ImGui::RadioButton("Floor", &selected, ArchitectureType::FLOOR); ImGui::SameLine();
	ImGui::RadioButton("Ramp", &selected, ArchitectureType::RAMP); ImGui::SameLine();
	ImGui::RadioButton("Up Stairs", &selected, ArchitectureType::UP_STAIRS); ImGui::SameLine();
	ImGui::RadioButton("Down Stairs", &selected, ArchitectureType::DOWN_STAIRS); ImGui::SameLine();
	ImGui::RadioButton("Up Down Stairs", &selected, ArchitectureType::UP_DOWN_STAIRS);

	if (mouse::rightClick)
		click1 = EMPTY_COORDINATES;

	// Dont allow designating through window since we use radio buttons here!
	if ((mouse::leftClick && !ImGui::IsMouseHoveringWindow()) || click1 != EMPTY_COORDINATES)
	{
		if (mouse::leftClick && click1 != EMPTY_COORDINATES)
			confirm = true;

		if (mouse::leftClick && !ImGui::IsMouseHoveringWindow())
			click1 = mouse::mousePos;

		if (mode == Mode::DRAW)
			drawDesignation();
		else
			drawErasure();
	}


	ImGui::End();
}

void checkTilesForValidity(Coordinates sml, Coordinates lrg, int type, std::function<void(bool, int, int)> onPossible)
{
	for (int x = sml.x; x <= lrg.x; ++x)
		for (int y = sml.y; y <= lrg.y; ++y)
		{
			bool aPossible = true;

			const int idx = getIdx({ x, y, sml.z });

			// Is location already designated for something?
			auto find = designations->architecture.find(idx);

			if (find != designations->architecture.end())
				aPossible = false;

			// Determine if architecture type is possible on tile
			else
			{
				using region::TileTypes;
				using region::solid;
				using region::Flag;
				using region::flag;

				bool flagConstruct = region::flag({ x, y, sml.z }, Flag::CONSTRUCTION);
				auto tileType = region::getTileType(idx);

				switch (type)
				{
				case ArchitectureType::WALL:
					if (solid(idx) || tileType != TileTypes::FLOOR || flagConstruct)
						aPossible = false;
					break;

				case ArchitectureType::FLOOR:
					if (solid(idx) || (tileType != TileTypes::FLOOR && tileType != TileTypes::EMPTY_SPACE) || flagConstruct)
						aPossible = false;
					break;

				case ArchitectureType::RAMP:
				case ArchitectureType::UP_STAIRS:
				case ArchitectureType::DOWN_STAIRS:
				case ArchitectureType::UP_DOWN_STAIRS:

					if (solid(idx) || tileType != TileTypes::FLOOR || flagConstruct)
						aPossible = false;
					break;

				}
			}

			// Run the passed function
			onPossible(aPossible, x, y);
		}
}

void DesignArchitecture::drawDesignation() // Add in the ability to select from availible materials!!! Add in ability to designate multiple levels of stairs simulatiniously!!
{
	using namespace mouse;

	vchars ch;
	switch (selected)
	{
	case ArchitectureType::WALL:
		ch = { WALL_1, {155, 155, 155}, {} };      
		break;

	case ArchitectureType::FLOOR:
		ch = { FLOOR_TEX,{ 155, 155, 155 },{} };   
		break;	

	case ArchitectureType::RAMP:
		ch = { RAMP_UP,{ 155, 155, 155 },{} };    
		break;

	case ArchitectureType::UP_STAIRS:
		ch = { STAIRS_UP,{ 155, 155, 155 },{} };    
		break;

	case ArchitectureType::DOWN_STAIRS:
		ch = { STAIRS_DOWN,{ 155, 155, 155 },{} };
		break;

	case ArchitectureType::UP_DOWN_STAIRS:
		ch = { STAIRS_UD,{ 155, 155, 155 },{} };
		break;	
	}
	

	Coordinates sml = mousePos;
	Coordinates lrg = { click1.x, click1.y, mouseZ };

	adjustCoordinatesForLoop(sml, lrg);

	std::vector<std::pair<int, int>> designationTiles;

	// Draw the potential designations
	int totalPossible = 0;
	overlayTerm->setAlpha(175);
	checkTilesForValidity(sml, lrg, selected, [&totalPossible, &ch](bool possible, int x, int y)
	{
		if (possible)
		{
			++totalPossible;
			overlayTerm->setChar(x, y, ch);
		}
		else
		{
			overlayTerm->setChar(x, y, { SQUARE_X_TEX,{ 255, 0, 0 },{ 100, 0, 0 } });
		}
	});

	std::stringstream ss;
	ss << "Coordinates: " << sml.x << "," << sml.y << "," << sml.z << " : " << lrg.x << "," << lrg.y << "," << lrg.z << "\n";
	ss << "Total Blocks Required: " << totalPossible;
	ImGui::Text(ss.str().c_str());

	if (confirm && totalPossible)
	{
		confirm = false;
		designate();
		click1 = EMPTY_COORDINATES;
	}
}

void DesignArchitecture::drawErasure()
{
	using namespace mouse;

	Coordinates sml = mousePos;
	Coordinates lrg = { click1.x, click1.y, mouseZ };

	adjustCoordinatesForLoop(sml, lrg);

	int totalPossible = 0;
	for (int x = sml.x; x <= lrg.x; ++x)
		for (int y = sml.y; y <= lrg.y; ++y)
		{
			bool possibleErasure = true;

			auto idx = getIdx({ x, y, sml.z });

			// Is location already designated for something?
			auto find = designations->architecture.find(idx);

			if (find == designations->architecture.end())
				possibleErasure = false;

			else
			{
				
			}
		}
}

void DesignArchitecture::designate()
{
	using namespace mouse;

	Coordinates sml = mousePos;
	Coordinates lrg = click1;
	int md = mode;

	adjustCoordinatesForLoop(sml, lrg); 

	checkTilesForValidity(sml, lrg, mode, [&sml, &md](bool possible, int x, int y)
	{
		auto idx = getIdx({ x, y, sml.z });
		if (possible)
		{
			designations->architecture[idx] = std::make_pair(md, 100.0);
		}
	});

	//emit(arc) NEED TO EMIT AN ARCHITECTURE CHANGED MESSAGE
}
