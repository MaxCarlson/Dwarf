#include "stdafx.h"
#include "DesignMining.h"
#include "Globals\game_states.h"
#include "Globals\TexDefines.h"
#include "mouse.h"
#include "KeyDampener.h"
#include "Raws\Materials.h"
#include "ECS\Messages\designation_message.h"
#include <imgui.h>
#include <DwarfRender.h>

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

	static std::string miningModesStr = "Mine (d)\0Channel (c)\0Ramp (r)\0Up Stair (u)\0Down Stair (j)\0Up/Down Stair (x)\0Erase Designation (e)";

	ImGui::Text("Right click to cancel designation");
	ImGui::Text("Mining Modes: ");
	ImGui::SameLine();
	ImGui::Combo("##MiningModes", &miningType, miningModesStr.c_str());

	if (mouse::rightClick)
	{
		click = EMPTY_COORDINATES;
	}

	if (mouse::leftClick || click != EMPTY_COORDINATES)
	{
		if (mouse::leftClick && click != EMPTY_COORDINATES)
			confirm = true;

		click = mouse::mousePos;

		drawPossibleMining();
	}

	ImGui::End();
}

void loopThroughPossibleMining(int type, Coordinates sml, Coordinates lrg, std::function<void(bool, int, int)> onPossible)
{
	adjustCoordinatesForLoop(sml, lrg);

	for(int x = sml.x; x <= lrg.x; ++x)
		for (int y = sml.y; y <= lrg.y; ++y)
		{

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
	
	int totalPossible = 0;
	loopThroughPossibleMining(miningType, click, mousePos, [&totalPossible, &ch](bool possible, int x, int y)
	{
		if (possible)
		{
			lterm->setChar(x, y, ch);
			++totalPossible;
		}
		else
		{

		}
	});
}

void DesignMining::designate()
{
}

void DesignMining::drawErasure()
{
}
