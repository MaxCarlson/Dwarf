#include "stdafx.h"
#include "ClickSystemGui.h"
#include "ECS\Systems\EntityPositionCache.h"
#include "ECS\Components\Sentients\AiWorkComponent.h"
#include "ECS\Components\Fighting\Drafted.h"
#include "ECS\Components\HealthComponent.h"
#include "ECS\Systems\helpers\PathFinding.h"
#include "ECS\Components\MovementComponent.h"
#include "Globals\game_states.h"
#include "KeyDampener.h"
#include "mouse.h"
#include "Globals\GlobalWorld.h"
#include <imgui.h>
#include <SFML\Window.hpp>

void ClickSystemGui::init()
{
}

void handleDraft(Entity &e)
{
	using namespace keys;
	using namespace mouse;
	ImGui::Text("Right Click target to attack");
	ImGui::Text("Or right click empty tile to move");

	if (rightClick)
	{
		auto& draft = e.getComponent<Drafted>();
		const auto& atPos = positionCache->get_location(mousePos);

		bool foundTarget = false;
		for (const auto& p : atPos)
		{
			auto entAtPos = world.getEntity(p);

			if (entAtPos.hasComponent<HealthComponent>() && entAtPos.hasComponent<PositionComponent>()) // TODO: Add in more checks before assigning ent as a target
			{
				const auto& tCo = entAtPos.getComponent<PositionComponent>().co;
				auto path = findPath(e.getComponent<PositionComponent>().co, tCo);

				if (!path->failed)
				{
					draft.step = Drafted::GOTO_TARGET;
					draft.targetId = p;
					draft.targetCo = tCo;// TODO: Add some visual info about target
					foundTarget = true;
					e.getComponent<MovementComponent>().path = path->path;
					break;
				}
			}
		}

		if (!foundTarget)
		{
			auto path = findPath(e.getComponent<PositionComponent>().co, mousePos);

			if (!path->failed)
			{
				auto& mov = e.getComponent<MovementComponent>(); 
				mov.path = path->path;
			}
		}
	}
}

void handlePawn(Entity &e)
{
	using namespace keys;
	ImGui::Text("'r' to Draft"); // TODO: Add a drafted icon to drafted Entities

	if (isKeyDown(sf::Keyboard::Key::R, true))
	{
		if (!e.hasComponent<Drafted>())
			e.addComponent<Drafted>();
		else
			e.removeComponent<Drafted>();

		e.activate();
	}

	if(e.hasComponent<Drafted>())
		handleDraft(e);


}

void ClickSystem::update(const double duration)
{
	using namespace mouse;
	ImGui::Begin("##ClickSystem");

	static std::vector<size_t> clickedOn;

	if (gameState != GameState::DESIGN)
	{
		if (leftClick)
		{
			clickedOn = positionCache->get_location(mousePos);
		}
	}

	static int selected = 0;
	for (const auto& e : clickedOn) // TODO: Add in a selected element so we can tab through multiple entities
	{
		auto ent = world.getEntity(e);

		if (ent.hasComponent<AiWorkComponent>())
		{
			handlePawn(ent);
			break;
		}
	}

	ImGui::End();
}
