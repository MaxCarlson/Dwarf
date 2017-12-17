#pragma once
#include <string>

struct Coordinates;
class Entity;

void loadRaws();

Entity spawnItemOnGround(const std::string &itemTag, const std::size_t material, const Coordinates co);
