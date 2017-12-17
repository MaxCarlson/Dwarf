#pragma once
#include <string>

struct MaterialDef;

MaterialDef * returnMaterial(const std::string &tag);
MaterialDef * returnMaterial(const int idx);

void readInMaterials();
void sanityCheckMaterials();

