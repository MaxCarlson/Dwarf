#pragma once
#include "../Component.h"
#include "../../Coordinates.h"

// !!!!!!!!!!!~~~~~~~~~~~~!!!!!!!!!!!!~~~~~~~~~~ REPLACE THIS

// Going to be used for filtering capable
// Entities, as well as storing current job info
class JobComponent : public Component
{
public:
	JobComponent() = default;
};

enum class Jobs
{
	NONE,
	MINER,
	MASON,
	WOODCUTTER,
	CARPENTER
};

static const std::vector<int> listOfAllJobsByIndex = 
{
	int(Jobs::NONE),
	int(Jobs::MINER),
	int(Jobs::MASON),
	int(Jobs::WOODCUTTER),
	int(Jobs::CARPENTER)
};

static const std::vector<std::string> listOfAllJobsByString =
{
	"None",
	"Miner",
	"Mason",
	"Woodcutter",
	"Carpenter"
};