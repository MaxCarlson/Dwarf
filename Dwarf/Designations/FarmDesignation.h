#pragma once


struct FarmInfo // Move this to a sepperate file. Make a file for designation types?
{
	FarmInfo() = default;
	FarmInfo(const int step, const size_t seedId, const std::string& seedType) : step(step), seedId(seedId), seedType(seedType) {}

	enum Steps
	{
		CLEAR,
		ADD_SOIL,
		PLANT,
		GROWING
	};

	int step = CLEAR;

	size_t seedId = 0;
	std::string seedType = "";

	// Progress that has been made
	// of a step
	double progress = 0.0;

	bool beingWorked = false;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(step, seedId, seedType, progress, beingWorked);
	}
};