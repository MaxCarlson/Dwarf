#pragma once
#include <string>
#include <vector>

struct BuildingDef;

BuildingDef * getBuilding(const std::string &tag);

const std::vector<std::string> get_all_building_def_names();

void readInBuildings() noexcept;
void sanityCheckBuildings() noexcept;
