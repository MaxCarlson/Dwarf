#include "stdafx.h"
#include "DeathSystem.h"
#include "Helpers\MessageQueue.h"
#include "ECS\Messages\something_died_message.h"

MessageQueue<something_died_message> deaths;

void DeathSystem::init()
{
	subscribe<something_died_message>([this](something_died_message &msg)
	{
		deaths.emplace({msg.id, msg.killer});
	});
}

void DeathSystem::update(const double duration)
{
	deaths.processAll([&](something_died_message &msg)
	{
		auto dying = getWorld().getEntity(msg.id);


		// TODO: Spawn corpse
		// TODO: Emit log message with info about death


		dying.kill();
	});
}
