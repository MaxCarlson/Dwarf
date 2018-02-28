#include "EntityFactory.h"

#include "ECS\EntityManager.h"
//#include "Engine.h"
#include "Globals\GlobalWorld.h"
#include "Globals\DwarfContainer.h"
#include "ECS\World.h"

#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\Components\HealthComponent.h"
#include "ECS\Components\MovementComponent.h"
#include "ECS\Components\JobComponent.h"

#include "ECS\Components\Sentients\Inventory.h"
#include "ECS\Components\Sentients\Stats.h"
#include "ECS\Components\Sentients\AiWorkComponent.h"
#include "ECS\Components\Sentients\Needs.h"

#include "ECS\Components\Fighting\CombatBase.h"
#include "ECS\Components\ai\View.h"
#include "Raws\ReadBodyParts.h"
#include "Raws\SkillReader.h"
#include <SFML\Graphics\Color.hpp>
EntityFactory::EntityFactory()
{
}

// Need to create ui interface for starting dwarves
// Also should probably store a vector of dwarves for qucik access
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

	calculateCombatBase(dwarf, 0);

	dwarf.activate();

	dwarfContainer.dwarves.emplace_back(dwarf);

	// Add dwarves to engine vector
	//engine->Dwarves.push_back(dwarf);

	return dwarf;
}

// TODO: Make a generic species constructor that takes a species and creates one of that species
