#pragma once

namespace std { class string; }
struct BodyPartDef;

BodyPartDef * getBodyPart(const std::string& tag) noexcept;

void readInBodyParts() noexcept;
void sanityCheckBodyParts() noexcept;