#include "stdafx.h"
#include "SpawnCreature.h"
#include "ECS\Messages\spawn_creature_message.h"
#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\MovementComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\Components\HealthComponent.h"
#include "ECS\Components\ai\View.h"
#include "ECS\Components\Fighting\CombatBase.h"
#include "ECS\Components\Sentients\AiWorkComponent.h"
#include "ECS\Components\Sentients\Name.h"
#include "ECS\Components\Sentients\Needs.h"
#include "ECS\Components\Sentients\Inventory.h"
#include "ECS\Components\Sentients\Stats.h"
#include "ECS\Components\helpers\skill.h"
#include "Raws\SkillReader.h"
#include "Raws\ReadSpecies.h"
#include "Raws\ReadBodyParts.h"
#include "Raws\Defs\SpeciesDef.h"
#include "Helpers\Rng.h"

void SpawnCreature::init()
{
	subscribe<spawn_creature_message>([this](spawn_creature_message &msg)
	{
		spawnCreature(msg);
	});
}

void SpawnCreature::update(const double duration)
{}

void SpawnCreature::spawnCreature(spawn_creature_message &msg)
{
	Entity ent = getWorld().createEntity();

	ent.addComponent<RenderComponent>(msg.ch);
	ent.addComponent<PositionComponent>(msg.co);

	// TODO: Rework this to rely on data from the message
	// instead of hardcoded here
	if (msg.playerControlled)
	{
		auto& work = ent.addComponent<AiWorkComponent>();

		std::unordered_map<std::string, attribute> attributes;
		for (const auto& att : allAttributes())
		{
			attributes[att] = attribute{};
		}
		
		std::unordered_map<std::string, skill> skills;
		for (const auto& sk : allSkills())
		{
			skills[sk] = skill{};
			work.jobPrefrences[sk] = 10;
		}

		ent.addComponent<Stats>(attributes, skills);
		ent.addComponent<Needs>();

	}

	ent.addComponent<MovementComponent>(msg.moveSpeed);

	auto& body = getSpecies(msg.speciesTag)->bodyTag; // TODO: Error handling if body doesn't exist!
	ent.addComponent<HealthComponent>(100, 100, 1, body, getCreatureBody(body));

	ent.addComponent<Inventory>();
	ent.addComponent<CombatBase>();
	ent.addComponent<View>(10);
	ent.addComponent<Name>(msg.fname, msg.lname);

	calculateCombatBase(ent, 0);

	ent.activate();

	//dwarfContainer.dwarves.emplace_back(dwarf);
}

Entity createDwarf(DwarfCreationObj dwarfConstruct)
{

	Entity dwarf = world.createEntity();

	dwarf.addComponent<RenderComponent>(vchar{ 769, color_from_name("default"), sf::Color::Transparent.toInteger() });
	dwarf.addComponent<PositionComponent>(dwarfConstruct.co);
	auto& work = dwarf.addComponent<AiWorkComponent>();

	std::unordered_map<std::string, attribute> attributes;

	for (const auto& att : allAttributes())
	{
		attributes[att] = attribute{};
	}
	std::unordered_map<std::string, skill> skills;


	for (const auto& sk : allSkills())
	{
		skills[sk] = skill{};
		work.jobPrefrences[sk] = 10;
	}

	dwarf.addComponent<Stats>(attributes, skills);

	// Movement speed in constructor in tiles per second
	dwarf.addComponent<MovementComponent>(3.5);

	static const std::string bodyDefTag = "human_body"; // TODO: Grab this from species
	dwarf.addComponent<HealthComponent>(100, 100, 1, bodyDefTag, getCreatureBody(bodyDefTag));

	dwarf.addComponent<Inventory>();
	dwarf.addComponent<Needs>();
	dwarf.addComponent<CombatBase>();
	dwarf.addComponent<View>(10);
	dwarf.addComponent<Name>(randomName(), randomName());

	calculateCombatBase(dwarf, 0);

	dwarf.activate();

	dwarfContainer.dwarves.emplace_back(dwarf);

	return dwarf;
}