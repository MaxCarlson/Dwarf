#pragma once
#include <string>

struct SpeciesDef;

SpeciesDef * getSpecies(const std::string& tag) noexcept;

void readInSpecies() noexcept;
void sanityCheckSpecies() noexcept;
