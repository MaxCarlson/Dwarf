#pragma once
#include "stdafx.h"

struct PlantDef;

PlantDef * getPlantIdx(const std::string& tag) noexcept;

PlantDef * getPlantDef(const size_t idx) noexcept;

void readInPlants() noexcept;

void sanityCheckPlants() noexcept;
