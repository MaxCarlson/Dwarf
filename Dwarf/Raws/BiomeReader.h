#pragma once

struct BiomeDef;

BiomeDef * getBiomeDef(const std::size_t idx);

void readInBiomes() noexcept;

void sanityCheckBiomes() noexcept;
