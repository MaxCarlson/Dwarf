#pragma once
#include "Reaction.h" // Forward declare if we get linker issues, this is just to avoid having to include reaction + reactionInput whenever we use the getReaction funcitons

Reaction * getReaction(const std::string tag);

const std::vector<Reaction>* getReactionsByWorkshop(const std::string tag);

void readInReactions() noexcept;

void sanityCheckReactions();


