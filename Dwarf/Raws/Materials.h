#pragma once
#include <string>
#include <vector>

struct MaterialDef;

MaterialDef * getMaterial(const std::string &tag);
MaterialDef * getMaterial(const std::size_t  idx);

const std::size_t getMaterialIdx(const std::string &tag);

const std::vector<std::string> getAllMaterialTags();

void readInMaterials() noexcept;
void sanityCheckMaterials();

void getStrataLayers(std::vector<std::size_t> &soils, std::vector<std::size_t> &sands, std::vector<std::size_t> &igneous, std::vector<std::size_t> &sedimentries, std::vector<std::size_t> &metamorphics);

