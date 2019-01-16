#include "raws.h"
#include "Lua.h"
#include "ItemRead.h"
#include "FileReader.h"
#include "Materials.h"
#include "Defs\MaterialDef.h"
#include "../Coordinates.h"
#include "Globals\GlobalWorld.h"
#include "../ECS/Components/PositionComponent.h"
#include "../ECS/Components/RenderComponent.h"
#include "../ECS/Components/Item.h"
#include "Defs\ItemDefs.h"
#include "Buildings.h"
#include "ReadReactions.h"
#include "ReadPlants.h"
#include "SkillReader.h"
#include "BiomeReader.h"
#include "ReadBodyParts.h"
#include "ReadSpecies.h"

void sanityChecks()
{
	sanityCheckSkills();
	sanityCheckStockpiles();
	sanityCheckItems();
	sanityCheckMaterials();
	sanityCheckBuildings();
	sanityCheckReactions();
	sanityCheckPlants();
	sanityCheckBiomes();
	sanityCheckBodyParts();
	sanityCheckBodyDefs();
	sanityCheckSpecies();
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
	readInBiomes();
	readInBodyParts();
	readyInBodyDefs();
	readInSpeciesDefs();
}

void loadRaws()
{
	using namespace FileStrings;

	auto ll = std::make_unique<LuaLife>();

	readFile(0, "Raws/LuaDefs/names.txt"); // TODO: Move into index
	readFile(1, "Raws/LuaDefs/index.txt");
	for (const std::string & fileName : returnTable(1)->strings)
	{
		loadLuaScript("Raws/LuaDefs/" + fileName);
	}

	loadGameTables();
	sanityChecks();
}

// TODO: Move to spawning class?
Entity spawnItemOnGround(const std::string itemTag, const std::size_t material, const Coordinates co, SpawnColor colorType, int quality)
{
	auto item = getItemDef(itemTag);

	auto mat = getMaterial(material);

	auto e = world.createEntity();

	e.addComponent<PositionComponent>(co);

	auto& rend = e.addComponent<RenderComponent>(RenderComponent{item->ch});

	e.addComponent<Item>(Item{ item->name, item->tag, item->categories, material, item->stackSize });
	e.addComponent<Quality>(quality);

	// SpawnColor::ITEM_COLOR is default

    if (colorType == SpawnColor::MATERIAL_COLOR)
	{
		rend.ch.fg = color_from_name(mat->color.c_str()); // Switch materials over to using vchar
	}

	e.activate();
	return e;
}
