#include "stdafx.h"
#include "ProcessAttacks.h"
#include "Helpers\MessageQueue.h"
#include "ECS\Messages\damage_entity_message.h"
#include "ECS\Components\HealthComponent.h"
#include "ECS\Messages\something_died_message.h"
#include "Helpers\Rng.h"

MessageQueue<damage_entity_message> attacks;

void ProcessAttacks::init()
{
	subscribe<damage_entity_message>([this](const damage_entity_message &msg)
	{
		attacks.emplace(damage_entity_message {msg.dmg, msg.attacker, msg.target});
	});
}

//inline void pickBodyPart(){} 

// TODO: Emit combat log of events
// TODO: Determine body part hit

void ProcessAttacks::update(const double duration)
{
	attacks.processAll([&](damage_entity_message &msg)
	{
		auto defender = getWorld().getEntity(msg.target);
		//auto attacker = getWorld().getEntity(msg.attacker);

		if (!defender.hasComponent<HealthComponent>())
		{
			std::cout << "Entity " << msg.target << " Has No Health Component!";
			return;
		}

		auto& health = defender.getComponent<HealthComponent>(); 

		health.health -= msg.dmg;

		if (health.health <= 0)
			emit(something_died_message { msg.attacker, msg.target });
	});
}
