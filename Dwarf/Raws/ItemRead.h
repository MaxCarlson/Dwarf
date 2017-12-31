#pragma once
#include <string>
#include <vector>
#include <functional>

struct ItemDef;
struct StockpileDef;

ItemDef * getItemDef(const std::string tag);

StockpileDef * getStockpileDef(const std::string tag);

void foreachStockpile(const std::function<void(StockpileDef *)>& func);

const std::vector<std::string> get_all_item_tags();

void readInItems() noexcept;
void sanityCheckItems() noexcept;

void readInStockpiles() noexcept;
void sanityCheckStockpiles() noexcept;
