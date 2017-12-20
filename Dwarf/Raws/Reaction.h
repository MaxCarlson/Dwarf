#pragma once
#include <string>
#include <vector>
#include "ReactionInput.h"

struct Reaction
{
	std::string tag = "";
	std::string name = "";
	std::string workshop = "";

	std::vector<ReactionInput> inputs;
	std::vector<std::pair<std::string, int>> outputs;
	std::string skill;

	//int difficulty;
};