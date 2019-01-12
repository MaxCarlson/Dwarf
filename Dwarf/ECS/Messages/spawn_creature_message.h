#pragma once
#include "ECS\Systems.h"
#include "Drawing\vchar.h"
#include "Coordinates.h"

struct spawn_creature_message : public base_message_t
{
	spawn_creature_message() = default;
	spawn_creature_message(std::string speciesTag, std::string fname, std::string lname,
		vchar ch, Coordinates co, float moveSpeed, bool playerControlled) :

		speciesTag{ speciesTag },
		fname{fname},
		lname{lname},
		ch{ch},
		co{co},
		moveSpeed{moveSpeed},
		playerControlled{playerControlled}
	{}

	std::string speciesTag;
	std::string fname;
	std::string lname;
	vchar ch;
	Coordinates co;
	float moveSpeed;
	bool playerControlled;
};