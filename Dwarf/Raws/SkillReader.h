#pragma once
#include <vector>
#include <string>

const int* attributeIdx(const std::string & attr);

const std::vector<std::string>* attributesBySkill(const std::string & skill);

void readInSkills() noexcept;
void sanityCheckSkills() noexcept;
