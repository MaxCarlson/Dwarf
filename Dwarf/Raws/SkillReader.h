#pragma once
#include <vector>
#include <string>

const std::vector<std::string>& allAttributes();

const std::vector<std::string>* attributesBySkill(const std::string & skill);

void readInSkills() noexcept;
void sanityCheckSkills() noexcept;
