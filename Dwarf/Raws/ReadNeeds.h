#pragma once

struct NeedDef;

NeedDef * getNeed(const std::string& tag);


void readInNeeds() noexcept;
void sanityCheckNeeds() noexcept;

