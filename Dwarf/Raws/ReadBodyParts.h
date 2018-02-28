#pragma once
#include <string>

struct BodyPartDef;
struct BodyDef;
struct BodyPart;

BodyPartDef * getBodyPart(const std::string& tag) noexcept;
BodyDef * getBodyDef(const std::string &tag) noexcept;
std::vector<BodyPart> getCreatureBody(const std::string &tag);

void readInBodyParts() noexcept;
void readyInBodyDefs() noexcept;

void sanityCheckBodyParts() noexcept;
void sanityCheckBodyDefs() noexcept;