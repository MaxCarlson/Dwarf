#pragma once
#include <string>

struct BodyPartDef;
struct BodyDef;

BodyPartDef * getBodyPart(const std::string& tag) noexcept;
BodyDef * getBodyDef(const std::string &tag) noexcept;

void readInBodyParts() noexcept;
void readyInBodyDefs() noexcept;

void sanityCheckBodyParts() noexcept;
void sanityCheckBodyDefs() noexcept;