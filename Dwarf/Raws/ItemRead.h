#pragma once
#include <string>
#include <vector>

struct ItemDef;

ItemDef * getItemDef(const std::string tag);

const std::vector<std::string> get_all_item_tags();

void readInItems() noexcept;
void sanityCheckItems() noexcept;
