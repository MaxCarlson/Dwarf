#pragma once
#include <string>

struct MaterialDef;

MaterialDef * getMaterial(const std::string &tag);
MaterialDef * getMaterial(const int idx);

const std::size_t getMaterialIdx(const std::string &tag);

void readInMaterials() noexcept;
void sanityCheckMaterials();

