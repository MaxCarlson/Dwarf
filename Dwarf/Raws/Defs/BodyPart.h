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

// These are used to define the layout of body parts
// that make up a species body
struct SpeciesBodyPart
{
	int id = 0;
	std::string tag = "";
	std::string attachedTo = "";
	int idAttachedTo = 0;

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