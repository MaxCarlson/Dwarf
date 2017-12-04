#include "DijkstraMapsHandler.h"
#include "PickMapSystem.h"
#include "../../Messages/pick_map_changed_message.h"
#include "../../World.h"

DijkstraMap pick_map;

void DijkstraMapsHandler::init()
{
	// Create systems
	pickMapSystem = std::make_unique<PickMapSystem>();

	// Subscribe to messages
	subscribe_mbox<pick_map_changed_message>();
}

void DijkstraMapsHandler::update()
{
	each_mbox<pick_map_changed_message>([this](const pick_map_changed_message &msg) { update_pick_map = true; });

	if (update_pick_map)
	{
		pickMapSystem->update();
	}
}
