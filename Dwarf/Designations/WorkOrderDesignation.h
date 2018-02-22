#pragma once
#include <memory>
#include <cereal\types\vector.hpp>

struct WorkOrderDesignation
{
	// Reaction tag
	std::string tag;

	// How many times should we do this?
	int count = 1;

	// What material should we make it out of?
	// 0 is does not matter
	size_t material = 0;

	// Is there a particular worker we'd like to 
	// have doing this?
	size_t workerId = 0;

	// Do we want workers of greater than this level
	// doing this?
	int minSkillLevel = 0;

	// TODO: Worth it to add a max skill level?

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(tag, count, material, workerId, minSkillLevel);
	}
};

struct AQWorkOrders
{
	std::vector<WorkOrderDesignation> active;
	std::vector<WorkOrderDesignation> queued;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(active, queued);
	}
};

extern AQWorkOrders workOrders;