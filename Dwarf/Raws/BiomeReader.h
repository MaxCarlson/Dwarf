#pragma once

struct BiomeDef;

BiomeDef * getBiomeDef(const std::size_t idx);

void forEachBiome(std::function<void(BiomeDef *)> func);

void readInBiomes() noexcept;

void sanityCheckBiomes() noexcept;
