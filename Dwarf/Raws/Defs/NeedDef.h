#pragma once
#include <functional>
#include <string>


struct NeedDef
{
	NeedDef() = default;

	std::string tag = "";
	std::string name = "";

	int happyReduction = 0;

	double declineRate = 0.0;

	// On what need level do we run the function
	double onLvl = 0.0;

	// How often is the function run?
	double howOften = 0.0;

	// Function that runs when need level hits 'onLvl'
	std::function<void(Entity)> onCritical = [](Entity) {};
};