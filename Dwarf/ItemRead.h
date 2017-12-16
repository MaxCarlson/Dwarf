#pragma once
#include <string>

struct ItemDef;

ItemDef * getItemDef(const std::string tag);

void readInItems() noexcept;
void sanityCheckItems() noexcept;
