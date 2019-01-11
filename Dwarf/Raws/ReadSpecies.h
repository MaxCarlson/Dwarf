#pragma once
#include <string>

struct SpeciesDef;

SpeciesDef * getSpecies(const std::string& tag) noexcept;

void readInSpeciesDefs() noexcept;
void sanityCheckSpecies() noexcept;
