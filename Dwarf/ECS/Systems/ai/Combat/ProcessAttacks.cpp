#include "stdafx.h"
#include "ProcessAttacks.h"
#include "Helpers\MessageQueue.h"
#include "ECS\Messages\damage_entity_message.h"
#include "ECS\Components\HealthComponent.h"
#include "ECS\Messages\something_died_message.h"
#include "Helpers\Rng.h"
#include "Raws\ReadBodyParts.h"
#include "Raws\Defs\BodyPart.h"
#include <random>

MessageQueue<damage_entity_message> attacks;

void ProcessAttacks::init()
{
	subscribe<damage_entity_message>([this](const damage_entity_message &msg)
	{
		attacks.emplace(damage_entity_message {msg.dmg, msg.attacker, msg.target});
	});
}

// TODO: Emit combat log of events
// TODO: Determine body part hit
// TODO: Add bleading + other damage over time into equation
//
// Returns the idx of the body part
// that's going to be hit
int pickBodyPartToHit(const BodyDef * def)
{
	static std::default_random_engine re;
	std::uniform_real_distribution<double> unif(0.0, def->maxChance); // TODO: Find a way to merge this with Rng class for deterministic random

	double output = unif(re);

	int i = 0;
	for (const auto& d : def->hitChances) // TODO: More effecient method of sloting body parts into hit %'s?
	{
		if (d > output)
			return i;

		++i;
	}
	std::cout << "Hit Chances Broken!! \n";
	return 0;
}

bool areAllPartsGone(const BodyDef * b, const HealthComponent &h, const int partIdx) // TODO: Move these functions to a sepperate file name organ/bodypart effects or something
{
	const auto& partTag = h.bodyParts[partIdx].tag;

	for (const auto& p : h.bodyParts)
		if (p.tag == partTag && p.health > 0.0)
			return false;
				
	return true;
}

// E.G. if an arm is destroyed, kill all parts connected down from it
// like the hands, fingers, any bones, etc
inline void recursivlyFindParts()
{

}

void destroyConnectedParts(BodyPart &part, HealthComponent &h, const int partIdx)
{
	auto* body = getBodyDef(h.bodyDefTag);
	if (!body)
		return;

	auto& partDef = body->parts[partIdx]; 

	for (const auto& p : body->parts)
	{
		
	}
}

inline void damagePart(const BodyDef * b, HealthComponent &h, const int partIdx, const double dmg)
{
	auto& hitPart = h.bodyParts[partIdx];

	if (hitPart.health <= 0) // TODO: Log Message stating miss
	{
		std::cout << "Would have hit but body part is gone! \n";
		return;
	}	

	// TODO: Armour considerations
	hitPart.health -= dmg;

	// TODO: When part is < 2/3rds health subtract it's health from main health?

	std::cout << "Entity hit another in: " << hitPart.tag << " for: " << dmg << "\n"; // TODO: Game Log

	// Process damage limbs and
	// the possible tree of other limbs attached to it
	if (hitPart.health < 0.0)
	{
		h.health -= getBodyPart(hitPart.tag)->health; // TODO: Create a better model for damage

		auto* partDef = getBodyPart(hitPart.tag); 

		if (partDef->effects.test(BodyPartEffects::ON_ALL_GONE_KILL) && areAllPartsGone(b, h, partIdx))
		{
			h.health = 0;
		}

		if (partDef->effects.test(BodyPartEffects::DAMAGE_MOBILITY))
		{

		}

		destroyConnectedParts(hitPart, h, partIdx);

		std::cout << "Entity lost: " << hitPart.tag << "\n"; // TODO: Replace this with a log
	}
}

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

		auto* bodyDef = getBodyDef(health.bodyDefTag);

		if (!bodyDef)
		{
			std::cout << "No Body Matches Tag in Process Attacks!!! \n";
			return;
		}

		int hitIdx = pickBodyPartToHit(bodyDef);

		damagePart(bodyDef, health, hitIdx, static_cast<double>(msg.dmg));


		//health.health -= msg.dmg;

		if (health.health <= 0)
			emit(something_died_message { msg.attacker, msg.target });
	});
}
