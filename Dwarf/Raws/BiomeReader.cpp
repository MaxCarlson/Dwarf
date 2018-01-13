#include "stdafx.h"
#include "BiomeReader.h"
#include "Defs\BiomeDef.h"
#include "Lua.h"

std::vector<BiomeDef> biomes;

BiomeDef * getBiomeDef(const std::size_t idx)
{
	return &biomes[idx];
}

void readInBiomes() noexcept
{

}

void sanityCheckBiomes() noexcept
{
}
