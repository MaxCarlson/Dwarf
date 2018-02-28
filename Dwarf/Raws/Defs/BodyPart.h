#pragma once
#include "stdafx.h"

struct BodyPart // Move this, this will be inside entity
{
	int idx = 0; // Where in this species body def vector is this parts def located?
	std::string tag; 

	double health = 1.0;
};

struct BodyPartDef
{
	std::string tag = "";
	std::string name = "";
	std::string description = "";

	int health = 1; // TODO: Make this relative to species?

	// TODO: const std::function<void(Entity)>& onDestoryed; 
	// TODO: const std::function<void(Entity)>& onDamage; 
};

struct BodyPartGroup
{

	double maxChance = 0.0;
	std::vector<double> hitChances; // Add % hit chances together to get # in this vector ~~ 
									// Roll between 0 and max ~~ Search through this vector for index > than roll. Use that index as index into parts
									// e.g. hit chances 4.0, 5.0, 6.0 make vector { 4.0, 9.0, 15.0 }
									// and roll between 0 and 4.0 is first part, etc. 
									// How to handle missing body parts? Should attack just miss if missing idx is hit? Yes.

	std::vector<BodyPartDef> parts; // Sort body parts from least likely to most
};

struct SpeciesBodyPart
{
	std::string tag = "";
	std::string attachedTo = "";
	double size = 0.0;

};

struct BodyDef
{
	std::string tag = "";

	// TODO: Figure out a way to assign a % hit chance to each part!
	double maxChance = 0.0;

	std::vector<double> hitChances;
	std::vector<SpeciesBodyPart> partGroups;
};