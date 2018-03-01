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
void decideDamageExtent(const Entity &attacker, const Entity &defender, const DamageTypes type, int &dmg)
{
	auto& attStats = attacker.getComponent<Stats>();
	auto& defStats = defender.getComponent<Stats>();

	const std::string attType = type == DamageTypes::MELEE ? "melee" : "ranged";

	// Find attackers attacking skill
	auto afind = attStats.skills.find(attType);
	if (afind == attStats.skills.end())
		return; 

	static const std::string dodgeStr = "dodging";
	static const std::string armourStr = "armour";
	auto dodgeFind = defStats.skills.find(dodgeStr);
	auto armourFind = defStats.skills.find(armourStr); // TODO: Add in shield stuff!

	auto dodge = dodgeFind != defStats.skills.end() ? dodgeFind->second.skillLvl : 0;
	auto armour = armourFind != defStats.skills.end() ? armourFind->second.skillLvl : 0;

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
	int dodgeDiff  = ( afind->second.skillLvl - dodge  ) / 2; // TODO: Make accuracy based on sight and attacker skill
	int armourDiff = ( afind->second.skillLvl - armour ) / 2;

	chances[int(AttackOutcomes::DODGED)]   -=  dodgeDiff;
	chances[int(AttackOutcomes::GLANCING)] -= armourDiff;

	std::uniform_real_distribution<double> unif(0.0, std::accumulate(chances.begin(), chances.end(), 0.0));

	double result = unif(randomEngine);

	int i = 0;
	double num = 0.0;
	for (i; i < chances.size(); ++i)
	{
		num += chances[i];

		if (num > result)
			break;
	}

	constexpr int baseXp = 2;

	// Reward xp and change damage based on
	// what type of hit this turned out to be
	int xp = baseXp;
	AttackOutcomes outcome = static_cast<AttackOutcomes>(i);
	switch (outcome) // TODO: Log Messages for these
	{
	case AttackOutcomes::MISSED:
		std::cout << "Entity totally missed attacking (other)! \n"; // TODO: Log with names
		xp -= 1;
		dmg = 0;
		break;
	case AttackOutcomes::DODGED:
		std::cout << "Entity dodged attacker! \n";
		giveWorkXp(defStats, dodgeStr, xp + 2);
		dmg = 0;
		break;
	case AttackOutcomes::BLOCKED:
		std::cout << "Entity blocked an attack! \n";
		// Add shield xp if defender has a shield (or not as well?)
		dmg *= 0.05;
		break;
	case AttackOutcomes::GLANCING:
		std::cout << "Attack is just a glancing blow \n";
		dmg *= 0.3;
		break;
	case AttackOutcomes::NORMAL:
		break;
	case AttackOutcomes::HARD:
		std::cout << "Attack hits harder than normal \n";
		++xp;
		dmg *= 1.25;
		break;
	case AttackOutcomes::CRITICAL:
		std::cout << "Entity hits critically \n";
		xp += 2;
		dmg *= 1.45;
		break;
	}

	giveWorkXp(attStats, attType, xp);

	// TODO: Give work xp to defender based on skills used
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

		// Figure out what happens with this hit, does it miss, etc
		decideDamageExtent(attacker, defender, msg.type, msg.dmg);

		if (msg.dmg <= 0) return;

		// Which part of the body does this attack hit?
		int hitIdx = pickBodyPartToHit(bodyDef);

		// Damage that part of the body and deal with removing 
		// attached limbs if part is killed
		damagePart(bodyDef, health, hitIdx, static_cast<double>(msg.dmg));

		if (health.health <= 0) // TODO : Bonus Xp for Kill?
			emit(something_died_message { msg.attacker, msg.target });
	});
}
