#include "raws.h"
#include "Lua.h"
#include "ItemRead.h"
#include "FileReader.h"
#include "Materials.h"
#include "../Coordinates.h"
#include "../Engine.h"
#include "../ECS/Components/PositionComponent.h"
#include "../ECS/Components/RenderComponent.h"
#include "../ECS/Components/Item.h"
#include "Defs\ItemDefs.h"
#include "Buildings.h"
#include "ReadReactions.h"
#include "ReadPlants.h"
#include "SkillReader.h"


void sanityChecks()
{
	sanityCheckSkills();
	sanityCheckStockpiles();
	sanityCheckItems();
	sanityCheckMaterials();
	sanityCheckBuildings();
	sanityCheckReactions();
	sanityCheckPlants();
}

void loadGameTables()
{
	readInSkills();
	readInStockpiles();
	readInItems();
	readInMaterials();
	readInBuildings();
	readInReactions();
	readInPlants();
}

void loadRaws()
{
	using namespace FileStrings;

	auto ll = std::make_unique<LuaLife>();

	readFile(0, "Raws/LuaDefs/index.txt");
	for (const std::string & fileName : returnTable(0)->strings)
	{
		loadLuaScript("Raws/LuaDefs/" + fileName);
	}

	loadGameTables();
	sanityChecks();
}

Entity spawnItemOnGround(const std::string itemTag, const std::size_t material, const Coordinates co)
{
	auto item = getItemDef(itemTag);

	auto mat = getMaterial(material);

	auto e = engine->world.createEntity();

	e.addComponent<PositionComponent>(co);

	e.addComponent<RenderComponent>(RenderComponent{item->ch});

	e.addComponent<Item>(Item{ item->name, item->tag, item->categories, material, item->stackSize });

	e.activate();

	return e;
}
