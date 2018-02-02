#include "stdafx.h"
#include "PlayGameLoop.h"
#include "WorldGenLoop.h"
#include "Globals\game_states.h"
#include "Globals\GlobalWorld.h"
#include "RunSystems.h"
#include "RegisterComponents.h"
//#include "Map\building\regionBuilder.h"
//#include "Helpers\Rng.h"
#include <imgui.h>
#include <imgui-SFML.h>

namespace Details
{
	//bool initNewGame = false;
}

std::unordered_map<std::string, SystemBase*> systems;

void newGame()
{
	using namespace Details;
}

void PlayGameLoop::run(const double duration)
{
	using namespace Details;

	if (gameState == GameState::NEW_GAME)
	{
		//ImGui::SetNextWindowPosCenter();
		//ImGui::Begin("Pick Region Size", nullptr, ImVec2{ 600, 400 }, 0.7f, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);

		ComponentsInit::init(); // This isn't strictly neccasary if a new game has been started before, probably won't cause issues but keep a look out!
		RunSystems::initSystems();

		gameState = GameState::PLAYING;
	}
	
	else if (gameState == GameState::LOAD_GAME)
	{

	}

	if(gameState == GameState::PLAYING)
		RunSystems::updateSystems(duration);
}


