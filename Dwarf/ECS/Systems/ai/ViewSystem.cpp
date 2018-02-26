#include "stdafx.h"
#include "ViewSystem.h"
#include "ECS\Messages\entity_moved_message.h"

std::unordered_set<size_t> dirtyEnts;

void ViewSystem::init()
{
	subscribe<entity_moved_message>([this](entity_moved_message &msg)
	{
		dirtyEnts.emplace(msg.e.getId().index);
	});
}

void ViewSystem::update(const double duration)
{
	for (const auto& e : getEntities())
	{
		auto& v = e.getComponent<View>();

		// Update view if entity has moved
		// or doesn't have any view cached
		if (v.visibleCache.empty() || dirtyEnts.find(e.getId().index) != dirtyEnts.end())
		{

		}

		// TODO: Reveal map that hasn't been revealed yet!
	}
}
