#include "stdafx.h"
#include "DesignArchitecture.h"
#include "Globals\game_states.h"
#include "ECS\Messages\designate_architecture_message.h"
#include "mouse.h"
#include "KeyDampener.h"
#include "Globals\TexDefines.h"
#include <imgui.h>
#include <DwarfRender.h>

#define ICON_FA_TIMES u8"\uf00d"

void DesignArchitecture::init()
{
}

void DesignArchitecture::update(const double duration)
{
	ImGui::Begin("Architecture##Design", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	std::string close = std::string(ICON_FA_TIMES) + " Close##DesignArch";
	
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
	
	if (mouse::rightClick)
		click1 = EMPTY_COORDINATES;


	if (mouse::leftClick || click1 != EMPTY_COORDINATES)
	{
		if (click1 == EMPTY_COORDINATES)
			click1 = mouse::mousePos;

		drawDesignation();
	}
	
	ImGui::RadioButton("(W)all", &selected, 0); ImGui::SameLine();
	ImGui::RadioButton("(U)p Stair", &selected, 1); ImGui::SameLine();
	ImGui::RadioButton("(D)own Stairr", &selected, 2); ImGui::SameLine();
	ImGui::RadioButton("(R)amp", &selected, 3); ImGui::SameLine();
	ImGui::RadioButton("(F)loor", &selected, 4); 

	drawDesignation();

	ImGui::End();
}

void DesignArchitecture::drawDesignation()
{
	int ch = 0;
	if (mode == Mode::DRAW)
	{
		switch (selected)
		{
		case 0:
			ch = WALL_1; break;
		case 1:
			ch = STAIRS_UP; break;
		case 2:
			ch = STAIRS_DOWN; break;
		case 3:
			ch = RAMP_UP; break;
		case 4:
			ch = FLOOR_TEX; break;
		}
	}
}
