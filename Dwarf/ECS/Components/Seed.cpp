#include "Seed.h"
#include "Raws\Materials.h"
#include "Raws\raws.h"
#include "Raws\ReadPlants.h"
#include "Raws\Defs\PlantDef.h"
#include "ECS\Components\Item.h"
#include "Coordinates.h"

void spawnSeeds(const int num, const Coordinates& co, const std::string & plantTag)
{
	static const std::string seedName = "seed";
	static const std::string matName = "organic";
	static const auto mat = getMaterialIdx(matName);

	const auto& name = getPlantDef(getPlantIdx(plantTag))->name;

	for (int i = 0; i < num; ++i)
	{
		auto seed = spawnItemOnGround(seedName, mat, co, SpawnColor::ITEM_COLOR);
		seed.getComponent<Item>().name = name + " Seed";
		seed.addComponent<Seed>(plantTag);
		seed.activate();
	}
}
