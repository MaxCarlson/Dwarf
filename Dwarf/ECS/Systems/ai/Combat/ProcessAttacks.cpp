#include "stdafx.h"
#include "ProcessAttacks.h"
#include "Helpers\MessageQueue.h"
#include "ECS\Messages\damage_entity_message.h"
#include "ECS\Components\HealthComponent.h"
#include "ECS\Messages\something_died_message.h"
#include "Helpers\Rng.h"
#include "Raws\ReadBodyParts.h"
#include "Raws\Defs\BodyPart.h"
#include "ECS\Components\Sentients\Stats.h"
#include <random>
#include <numeric>

MessageQueue<damage_entity_message> attacks;

static std::default_random_engine randomEngine; // TODO: Encorporate into rng class


void ProcessAttacks::init()
{
	subscribe<damage_entity_message>([this](const damage_entity_message &msg)
	{
		attacks.emplace(damage_entity_message {msg.dmg, msg.attacker, msg.target, msg.type});
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
	std::uniform_real_distribution<double> unif(0.0, def->maxChance); // TODO: Find a way to merge this with Rng class for deterministic random

	double output = unif(randomEngine);

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
inline void recursivlyFindAndDestroyParts(const BodyDef &body, HealthComponent &h, const int id)
{
	std::set<int> foundParts = { id };
	for (const auto& p : body.parts)
	{
		if (p.idAttachedTo == id && h.bodyParts[p.id].health > 0.0)
		{
			h.bodyParts[p.id].health = 0.0;

			// Don't want to do a recursive search for connected parts
			// on parts we've already searched for. Probably pre-mature optomization but felt ugly without it
			if(foundParts.find(p.idAttachedTo) == foundParts.end())
				recursivlyFindAndDestroyParts(body, h, p.id);

			foundParts.emplace(p.idAttachedTo);
		}
	}
}

void destroyConnectedParts(BodyPart &part, HealthComponent &h, const int partIdx)
{
	auto* body = getBodyDef(h.bodyDefTag);
	if (!body)
		return;

	auto& partDef = body->parts[partIdx]; 

	recursivlyFindAndDestroyParts(*body, h, partIdx);
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
		hitPart.health = 0.0;

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

enum class AttackOutcomes
{
	MISSED,
	DODGED,
	BLOCKED,
	GLANCING,
	NORMAL,
	HARD,
	CRITICAL,
};

// Look at differences between entity armour, attack, dodge, etc skills
// and determine the extent of the damage done
AttackOutcomes decideDamageExtent(const Entity &attacker, const Entity &defender, const DamageTypes type, int &dmg)
{
	auto& attStats = attacker.getComponent<Stats>();
	auto& defStats = defender.getComponent<Stats>();

	const std::string attType = type == DamageTypes::MELEE ? "melee" : "ranged";

	// Find attackers attacking skill
	auto afind = attStats.skills.find(attType);
	if (afind == attStats.skills.end())
		return; 

	auto dodgeFind = defStats.skills.find("dodge");
	auto armourFind = defStats.skills.find("armour"); // TODO: Add in shield stuff!

	// For entities of identical defensive skill type and attack skill levels
	// 15% miss
	// 15% dodge chance 
	// 15% glancing
	// 35% normal hit
	// 15% hard
	// 5% critical

	std::vector<double> chances = { 15.0, 15.0, 15.0, 35.0, 15.0, 5.0 };

	// Give moderate bonuses or penalities to dodging / blocking / glancing 
	// based on stat differences between parties
	int dodgeDiff  = ( afind->second.skillLvl - dodgeFind->second.skillLvl ) / 2; // TODO: Make accuracy based on sight and attacker skill
	int armourDiff = ( afind->second.skillLvl - armourFind->second.skillLvl) / 2;

	chances[int(AttackOutcomes::DODGED)]   -=  dodgeDiff;
	chances[int(AttackOutcomes::GLANCING)] -= armourDiff;

	std::uniform_real_distribution<double> unif(0.0, std::accumulate(chances.begin(), chances.end(), 0.0));

	double result = unif(randomEngine);

	int i = 0;
	for (const auto& c : chances)
	{
		if (c > result)
			break;
		++i;
	}

	AttackOutcomes outcome = static_cast<AttackOutcomes>(i);

	switch (outcome)
	{
	case AttackOutcomes::MISSED:

		break;

	case AttackOutcomes::DODGED:

		break;

	case AttackOutcomes::BLOCKED:

		break;

	case AttackOutcomes::GLANCING:
		
		break;

	case AttackOutcomes::NORMAL:
		break;
	case AttackOutcomes::HARD:
		break;
	case AttackOutcomes::CRITICAL:
		break;
	}

	constexpr int xp = 2;
	giveWorkXp(attStats, attType, xp);

	// TODO: Give work xp to attacker based on quality of hit 

	// TODO: Give work xp to defender based on skills used

	return outcome;
}

void ProcessAttacks::update(const double duration)
{
	attacks.processAll([&](damage_entity_message &msg)
	{
		auto attacker = getWorld().getEntity(msg.attacker);
		auto defender = getWorld().getEntity(msg.target);

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

		if (health.health <= 0) // TODO : Bonus Xp for Kill?
			emit(something_died_message { msg.attacker, msg.target });
	});
}
