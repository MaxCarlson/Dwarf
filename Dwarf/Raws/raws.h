#pragma once
#include <string>
#include "ECS\Components\Quality.h"

struct Coordinates;

void loadRaws();

enum SpawnColor
{
	ITEM_COLOR,
	MATERIAL_COLOR,
};

Entity spawnItemOnGround(const std::string itemTag, const std::size_t material, const Coordinates co, SpawnColor colorType, int quality = QUALITY_NONE);
