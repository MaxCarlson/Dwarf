#pragma once
#include "stdafx.h"

struct PlantDef;

size_t getPlantIdx(const std::string& tag) noexcept;

PlantDef * getPlantDef(const size_t idx) noexcept;

std::vector<PlantDef> * getAllPlantDefs() noexcept; // Delete this once plants are localized to biomes

void readInPlants() noexcept;

void sanityCheckPlants() noexcept;
