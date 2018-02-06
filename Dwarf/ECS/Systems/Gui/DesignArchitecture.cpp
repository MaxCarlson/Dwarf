#include "stdafx.h"
#include "DesignArchitecture.h"
#include "Globals\game_states.h"
#include "ECS\Messages\designate_architecture_message.h"
#include "mouse.h"
#include "Map\Tile.h"
#include "KeyDampener.h"
#include "Globals\TexDefines.h"
#include "Designations.h"
#include <imgui.h>
#include <DwarfRender.h>


void DesignArchitecture::init()
{
}

void DesignArchitecture::update(const double duration)
{
	ImGui::Begin("Architecture##Design", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	std::string close = " Close##DesignArch";
	
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
		if (mouse::leftClick && !ImGui::IsMouseHoveringWindow())
			click1 = mouse::mousePos;

		if (mode == Mode::DRAW)
			drawDesignation();
		else
			drawErasure();
	}


	ImGui::End();
}

void DesignArchitecture::drawDesignation() // Add in the ability to select from availible materials!!! Add in ability to designate multiple levels of stairs simulatiniously!!
{
	using namespace mouse;

	vchars ch;
	if (mode == Mode::DRAW)
	{
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
	}


	int x1 = mouseX;
	int x2 = click1.x;
	int y1 = mouseY;
	int y2 = click1.y;
	int z = mouseZ;

	// Flip coordinates
	if (x1 > x2)
	{
		int tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	if (y1 > y2)
	{
		int tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	static auto* lterm = dfr::term(1);
	lterm->setAlpha(220);

	// Draw the potential designations
	int totalPossible = 0;
	for(int x = x1; x <= x2; ++x)
		for (int y = y1; y <= y2; ++y)
		{
			bool aPossible = true;

			const int idx = getIdx({ x, y, z });

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

				bool flagConstruct = region::flag({ x, y, z }, Flag::CONSTRUCTION);
				auto tileType = region::getTileType(idx);

				switch (selected)
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

			
			if (aPossible)
			{
				++totalPossible;
				lterm->setChar(x, y, ch);
			}
			else
			{
				lterm->setChar(x, y, { SQUARE_X_TEX, {255, 0, 0}, {100, 0, 0} });
			}
		}

	std::stringstream ss;
	ss << "Coordinates: " << x1 << "," << y1 << "," << z << " : " << x2 << "," << y2 << "," << z << "\n";
	ss << "Total Blocks Required: " << totalPossible;
	ImGui::Text(ss.str().c_str());

	if (totalPossible)
	{
		emit(designate_architecture_message{ selected , std::make_pair(getIdx({x1, y1, z}), getIdx({x2, y2, z})) }); // Perhaps look into 3d designating once we have a 3d view
	}
}

void DesignArchitecture::drawErasure()
{
}
