#pragma once
#include <string>

struct Coordinates;
class Entity;

void loadRaws();

enum SpawnColor
{
	ITEM_COLOR,
	MATERIAL_COLOR,
	CUSTOM_COLOR
};

Entity spawnItemOnGround(const std::string itemTag, const std::size_t material, const Coordinates co, SpawnColor colorType, uint32_t col = 0);
