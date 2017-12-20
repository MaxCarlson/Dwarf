#pragma once
#include <string>

struct BuildingDef;

BuildingDef * getBuilding(const std::string &tag);

void readInBuildings() noexcept;
void sanityCheckBuildings() noexcept;
